#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include "Graph.hpp"

#ifdef __APPLE__
#include <mach/mach.h>
#endif

using namespace std;

// 获取当前进程内存占用 (MB)
double getMemoryUsageMB() {
#ifdef __APPLE__
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO,
                  (task_info_t)&info, &infoCount) == KERN_SUCCESS) {
        return info.resident_size / (1024.0 * 1024.0);
    }
#endif
#ifdef __linux__
    // Linux: 读取 /proc/self/status
    ifstream status("/proc/self/status");
    string line;
    while (getline(status, line)) {
        if (line.substr(0, 6) == "VmRSS:") {
            long kb = 0;
            sscanf(line.c_str(), "VmRSS: %ld kB", &kb);
            return kb / 1024.0;
        }
    }
#endif
    return -1.0;
}

void printUsage(const char *prog) {
    cout << "DSCAN - Directed Structural Clustering\n\n";
    cout << "用法:\n";
    cout << "  " << prog << " <数据目录> index                              建立索引\n";
    cout << "  " << prog << " <数据目录> query <eps_c> <eps_f> <mu>          聚类查询\n";
    cout << "  " << prog << " <数据目录> update                              动态更新测试\n";
    cout << "  " << prog << " <数据目录> scalability [eps_c eps_f mu]        可扩展性测试\n\n";
    cout << "示例:\n";
    cout << "  " << prog << " data/cora index\n";
    cout << "  " << prog << " data/cora query 0.1 0.1 2\n";
    cout << "  " << prog << " data/scale_10k scalability 0.1 0.1 2\n";
}

int main(int argc, const char * argv[]) {
    if (argc < 3) {
        printUsage(argv[0]);
        return 1;
    }

    const char *dataDir = argv[1];
    const char *mode = argv[2];

    clock_t startTime, endTime;
    Graph *graph = new Graph(dataDir);

    if (strcmp(mode, "index") == 0) {
        // 模式1: 建立索引
        graph->readGraph();

        startTime = clock();
        graph->creatIndex();
        endTime = clock();
        cout << "索引构建完成 (" << string(dataDir) << "): "
             << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s\n";

    } else if (strcmp(mode, "query") == 0) {
        // 模式2: 聚类查询
        if (argc < 6) {
            cerr << "错误: query 模式需要 3 个参数: <eps_c> <eps_f> <mu>\n";
            cerr << "示例: " << argv[0] << " data/cora query 0.1 0.1 2\n";
            delete graph;
            return 1;
        }

        float eps_c = atof(argv[3]);
        float eps_f = atof(argv[4]);
        int mu = atoi(argv[5]);

        startTime = clock();
        graph->readIndex();
        endTime = clock();
        cout << "索引读取: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s\n";

        cout << "查询参数: eps_c=" << eps_c << ", eps_f=" << eps_f << ", mu=" << mu << "\n";

        startTime = clock();
        graph->query4(eps_c, eps_f, mu);
        endTime = clock();
        cout << "查询耗时: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s\n";

        graph->output(eps_c, eps_f, mu);
        cout << "结果已保存到: " << string(dataDir) << "/output/\n";

    } else if (strcmp(mode, "update") == 0) {
        // 模式3: 动态更新测试
        int n_ = graph->readGraph_update();
        graph->readIndex_update();

        double all_time_insert = 0, all_time_remove = 0;
        double all_time_insert_na = 0, all_time_remove_na = 0;

        for (int i = 0; i < 1000; i++) {
            int begin_v = rand() % (n_ - 1);
            int end_v = rand() % (n_ - 1);

            startTime = clock();
            graph->insert_na(begin_v, end_v);
            endTime = clock();
            all_time_insert_na += (double)(endTime - startTime);

            startTime = clock();
            graph->remove_na(begin_v, end_v);
            endTime = clock();
            all_time_remove_na += (double)(endTime - startTime);

            startTime = clock();
            graph->insert(begin_v, end_v);
            endTime = clock();
            all_time_insert += (double)(endTime - startTime);

            startTime = clock();
            graph->remove(begin_v, end_v);
            endTime = clock();
            all_time_remove += (double)(endTime - startTime);
        }

        cout << "all_time_insert:    " << all_time_insert / CLOCKS_PER_SEC << "s\n";
        cout << "all_time_remove:    " << all_time_remove / CLOCKS_PER_SEC << "s\n";
        cout << "all_time_insert_na: " << all_time_insert_na / CLOCKS_PER_SEC << "s\n";
        cout << "all_time_remove_na: " << all_time_remove_na / CLOCKS_PER_SEC << "s\n";

    } else if (strcmp(mode, "scalability") == 0) {
        // 模式4: 可扩展性测试 — 分阶段详细计时 + 内存报告
        // 可选参数: eps_c eps_f mu (若提供则也测 query)
        bool doQuery = (argc >= 6);
        float eps_c = doQuery ? atof(argv[3]) : 0.1;
        float eps_f = doQuery ? atof(argv[4]) : 0.1;
        int mu = doQuery ? atoi(argv[5]) : 2;

        double mem_base = getMemoryUsageMB();

        // ── Phase 1: 读图 ──
        auto t0 = chrono::high_resolution_clock::now();
        graph->readGraph();
        auto t1 = chrono::high_resolution_clock::now();
        double read_time = chrono::duration<double>(t1 - t0).count();
        double mem_after_read = getMemoryUsageMB();

        // ── Phase 2: 构建索引 ──
        auto t2 = chrono::high_resolution_clock::now();
        graph->creatIndex();
        auto t3 = chrono::high_resolution_clock::now();
        double index_time = chrono::duration<double>(t3 - t2).count();
        double mem_after_index = getMemoryUsageMB();

        // ── Phase 3: 读索引 ──
        Graph *graph2 = new Graph(dataDir);
        auto t4 = chrono::high_resolution_clock::now();
        graph2->readIndex();
        auto t5 = chrono::high_resolution_clock::now();
        double load_index_time = chrono::duration<double>(t5 - t4).count();
        double mem_after_load = getMemoryUsageMB();

        // ── Phase 4: 查询 (可选) ──
        double query_time = -1.0;
        if (doQuery) {
            auto t6 = chrono::high_resolution_clock::now();
            graph2->query4(eps_c, eps_f, mu);
            auto t7 = chrono::high_resolution_clock::now();
            query_time = chrono::duration<double>(t7 - t6).count();
            graph2->output(eps_c, eps_f, mu);
        }
        double mem_final = getMemoryUsageMB();

        // ── 输出结果 (CSV 友好格式) ──
        cout << "\n=== SCALABILITY RESULTS ===\n";
        cout << "DATASET: " << string(dataDir) << "\n";
        cout << "READ_TIME: " << read_time << "\n";
        cout << "INDEX_TIME: " << index_time << "\n";
        cout << "LOAD_INDEX_TIME: " << load_index_time << "\n";
        if (doQuery) {
            cout << "QUERY_TIME: " << query_time << "\n";
            cout << "QUERY_PARAMS: " << eps_c << " " << eps_f << " " << mu << "\n";
        }
        cout << "TOTAL_TIME: " << (read_time + index_time) << "\n";
        cout << "MEM_BASE_MB: " << mem_base << "\n";
        cout << "MEM_AFTER_READ_MB: " << mem_after_read << "\n";
        cout << "MEM_AFTER_INDEX_MB: " << mem_after_index << "\n";
        cout << "MEM_AFTER_LOAD_MB: " << mem_after_load << "\n";
        cout << "MEM_FINAL_MB: " << mem_final << "\n";
        cout << "=== END ===\n";

        delete graph2;

    } else {
        cerr << "错误: 未知模式 '" << mode << "'\n\n";
        printUsage(argv[0]);
        delete graph;
        return 1;
    }

    delete graph;
    return 0;
}





