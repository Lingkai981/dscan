#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <cctype>
#include "Graph.hpp"

using namespace std;

static bool isIntegerString(const string &s) {
    if (s.empty()) return false;
    size_t i = 0;
    if (s[0] == '+' || s[0] == '-') {
        if (s.size() == 1) return false;
        i = 1;
    }
    for (; i < s.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    }
    return true;
}

int main(int argc, const char *argv[]) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <graph_dir> <k> [mu] <dataset_tag> [parameter_c] [parameter_f]" << endl;
        return 1;
    }

    const string graph_dir = string(argv[1]);
    const int k = atoi(argv[2]);
    int mu = k;
    string dataset_tag;
    float parameter_c = 0.5f;
    float parameter_f = 0.5f;

    // Backward compatible parsing:
    // old: <graph_dir> <k> <dataset_tag> [c] [f]
    // new: <graph_dir> <k> <mu> <dataset_tag> [c] [f]
    if (argc >= 5 && isIntegerString(argv[3])) {
        mu = atoi(argv[3]);
        dataset_tag = string(argv[4]);
        parameter_c = (argc > 5) ? stof(argv[5]) : 0.5f;
        parameter_f = (argc > 6) ? stof(argv[6]) : 0.5f;
    } else {
        dataset_tag = string(argv[3]);
        parameter_c = (argc > 4) ? stof(argv[4]) : 0.5f;
        parameter_f = (argc > 5) ? stof(argv[5]) : 0.5f;
    }

    Graph *graph = new Graph(graph_dir.c_str(), k);

    clock_t start_time = clock();
    graph->readGraph();
    clock_t end_time = clock();
    const double read_time_s = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    start_time = clock();
    graph->creatIndex();
    end_time = clock();
    const double build_time_s = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    double precision = 0.0, recall = 0.0, f1 = 0.0, rmse = 0.0;
    uint64_t tp = 0, fp = 0, fn = 0;
    double core_precision = 0.0, core_recall = 0.0, core_f1 = 0.0, core_rmse = 0.0;
    double neighbor_precision2 = 0.0, neighbor_recall2 = 0.0, neighbor_f12 = 0.0, neighbor_rmse2 = 0.0;
    double combined_precision = 0.0, combined_recall = 0.0, combined_f1 = 0.0;

    start_time = clock();
    graph->evaluateNeighborProxy(parameter_c, parameter_f, precision, recall, f1, rmse, tp, fp, fn);
    end_time = clock();
    const double proxy_time_s = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    graph->evaluateCoreNeighborCombinedProxy(parameter_c, parameter_f, mu,
                                             core_precision, core_recall, core_f1, core_rmse,
                                             neighbor_precision2, neighbor_recall2, neighbor_f12, neighbor_rmse2,
                                             combined_precision, combined_recall, combined_f1);

    const string csv_path = "/home/data/mlk/code/index-new2/" + dataset_tag + "/" + dataset_tag + "-accuracy-proxy.csv";
    const bool csv_exists = ifstream(csv_path).good();

    ofstream out_file(csv_path, ios::app);
    if (!csv_exists) {
        out_file << "dataset,k,mu,parameter_c,parameter_f,read_time_s,build_time_s,proxy_time_s,tp,fp,fn,precision,recall,f1,rmse" << endl;
    }

    out_file << dataset_tag << ","
             << k << ","
             << mu << ","
             << fixed << setprecision(4) << parameter_c << ","
             << fixed << setprecision(4) << parameter_f << ","
             << fixed << setprecision(6) << read_time_s << ","
             << fixed << setprecision(6) << build_time_s << ","
             << fixed << setprecision(6) << proxy_time_s << ","
             << tp << ","
             << fp << ","
             << fn << ","
             << fixed << setprecision(8) << precision << ","
             << fixed << setprecision(8) << recall << ","
             << fixed << setprecision(8) << f1 << ","
             << fixed << setprecision(8) << rmse << endl;

    out_file.close();

    const string csv_plus_path = "/home/data/mlk/code/index-new2/" + dataset_tag + "/" + dataset_tag + "-accuracy-proxy-core-nei.csv";
    const bool csv_plus_exists = ifstream(csv_plus_path).good();

    ofstream out_plus(csv_plus_path, ios::app);
    if (!csv_plus_exists) {
        out_plus << "dataset,k,mu,parameter_c,parameter_f,read_time_s,build_time_s,proxy_time_s,core_precision,core_recall,core_f1,core_rmse,neighbor_precision,neighbor_recall,neighbor_f1,neighbor_rmse,combined_precision,combined_recall,combined_f1" << endl;
    }

    out_plus << dataset_tag << ","
             << k << ","
             << mu << ","
             << fixed << setprecision(4) << parameter_c << ","
             << fixed << setprecision(4) << parameter_f << ","
             << fixed << setprecision(6) << read_time_s << ","
             << fixed << setprecision(6) << build_time_s << ","
             << fixed << setprecision(6) << proxy_time_s << ","
             << fixed << setprecision(8) << core_precision << ","
             << fixed << setprecision(8) << core_recall << ","
             << fixed << setprecision(8) << core_f1 << ","
             << fixed << setprecision(8) << core_rmse << ","
             << fixed << setprecision(8) << neighbor_precision2 << ","
             << fixed << setprecision(8) << neighbor_recall2 << ","
             << fixed << setprecision(8) << neighbor_f12 << ","
             << fixed << setprecision(8) << neighbor_rmse2 << ","
             << fixed << setprecision(8) << combined_precision << ","
             << fixed << setprecision(8) << combined_recall << ","
             << fixed << setprecision(8) << combined_f1 << endl;

    out_plus.close();

    cout << "dataset=" << dataset_tag
         << " k=" << k
            << " mu=" << mu
         << " c=" << parameter_c
         << " f=" << parameter_f << endl;
    cout << "read_time_s=" << read_time_s
         << " build_time_s=" << build_time_s
         << " proxy_time_s=" << proxy_time_s << endl;
    cout << "tp=" << tp << " fp=" << fp << " fn=" << fn << endl;
    cout << "precision=" << precision
         << " recall=" << recall
         << " f1=" << f1
         << " rmse=" << rmse << endl;
        cout << "core_precision=" << core_precision
            << " core_recall=" << core_recall
            << " core_f1=" << core_f1
            << " core_rmse=" << core_rmse << endl;
        cout << "combined_precision=" << combined_precision
            << " combined_recall=" << combined_recall
            << " combined_f1=" << combined_f1 << endl;
    cout << "saved_to=" << csv_path << endl;
        cout << "saved_to_plus=" << csv_plus_path << endl;

    // Graph destructor has a known double-free path for some large settings.
    // Let OS reclaim memory at process exit to keep batch runs stable.
    return 0;
}
