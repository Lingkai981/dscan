#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <sstream>
#include "Graph.hpp"

using namespace std;

static vector<int> parseIntCsv(const string &s) {
    vector<int> out;
    string token;
    stringstream ss(s);
    while (getline(ss, token, ',')) {
        if (!token.empty()) out.push_back(stoi(token));
    }
    return out;
}

static vector<float> parseFloatCsv(const string &s) {
    vector<float> out;
    string token;
    stringstream ss(s);
    while (getline(ss, token, ',')) {
        if (!token.empty()) out.push_back(stof(token));
    }
    return out;
}

int main(int argc, const char *argv[]) {
    if (argc < 7) {
        cerr << "Usage: " << argv[0]
             << " <graph_dir> <k> <dataset_tag> <mu_csv> <cf_csv> <output_csv>" << endl;
        return 1;
    }

    const string graph_dir = string(argv[1]);
    const int k = atoi(argv[2]);
    const string dataset_tag = string(argv[3]);
    const vector<int> mu_list = parseIntCsv(string(argv[4]));
    const vector<float> cf_list = parseFloatCsv(string(argv[5]));
    const string output_csv = string(argv[6]);

    if (mu_list.empty() || cf_list.empty()) {
        cerr << "mu list or c/f list is empty." << endl;
        return 1;
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

    const bool csv_exists = ifstream(output_csv).good();
    ofstream out(output_csv, ios::app);
    if (!csv_exists) {
        out << "dataset,k,mu,parameter_c,parameter_f,read_time_s,build_time_s,co_query_time_s,no_query_time_s,"
            << "neighbor_precision,neighbor_recall,neighbor_f1,neighbor_rmse,"
            << "core_precision,core_recall,core_f1,core_rmse,combined_precision,combined_recall,combined_f1"
            << endl;
    }

    for (int mu : mu_list) {
        for (float c : cf_list) {
            for (float f : cf_list) {
                clock_t co_st = clock();
                graph->query_new_index(c, f, mu);
                graph->output(c, f, mu);
                clock_t co_ed = clock();
                const double co_query_time_s = (double)(co_ed - co_st) / CLOCKS_PER_SEC;

                clock_t no_st = clock();
                graph->query_na(c, f, mu);
                graph->output(c, f, mu);
                clock_t no_ed = clock();
                const double no_query_time_s = (double)(no_ed - no_st) / CLOCKS_PER_SEC;

                double precision = 0.0, recall = 0.0, f1 = 0.0, rmse = 0.0;
                uint64_t tp = 0, fp = 0, fn = 0;
                graph->evaluateNeighborProxy(c, f, precision, recall, f1, rmse, tp, fp, fn);

                double core_precision = 0.0, core_recall = 0.0, core_f1 = 0.0, core_rmse = 0.0;
                double neighbor_precision2 = 0.0, neighbor_recall2 = 0.0, neighbor_f12 = 0.0, neighbor_rmse2 = 0.0;
                double combined_precision = 0.0, combined_recall = 0.0, combined_f12 = 0.0;
                graph->evaluateCoreNeighborCombinedProxy(c, f, mu,
                                                         core_precision, core_recall, core_f1, core_rmse,
                                                         neighbor_precision2, neighbor_recall2, neighbor_f12, neighbor_rmse2,
                                                         combined_precision, combined_recall, combined_f12);

                out << dataset_tag << ","
                    << k << ","
                    << mu << ","
                    << fixed << setprecision(4) << c << ","
                    << fixed << setprecision(4) << f << ","
                    << fixed << setprecision(6) << read_time_s << ","
                    << fixed << setprecision(6) << build_time_s << ","
                    << fixed << setprecision(6) << co_query_time_s << ","
                    << fixed << setprecision(6) << no_query_time_s << ","
                    << fixed << setprecision(8) << neighbor_precision2 << ","
                    << fixed << setprecision(8) << neighbor_recall2 << ","
                    << fixed << setprecision(8) << neighbor_f12 << ","
                    << fixed << setprecision(8) << neighbor_rmse2 << ","
                    << fixed << setprecision(8) << core_precision << ","
                    << fixed << setprecision(8) << core_recall << ","
                    << fixed << setprecision(8) << core_f1 << ","
                    << fixed << setprecision(8) << core_rmse << ","
                    << fixed << setprecision(8) << combined_precision << ","
                    << fixed << setprecision(8) << combined_recall << ","
                    << fixed << setprecision(8) << combined_f12
                    << endl;

                cout << "dataset=" << dataset_tag
                     << " k=" << k
                     << " mu=" << mu
                     << " c=" << c
                     << " f=" << f
                     << " co_time=" << co_query_time_s
                     << " no_time=" << no_query_time_s
                     << endl;
            }
        }
    }

    out.close();
    // Graph destructor has a known double-free path for some large settings.
    // Let OS reclaim memory at process exit to keep batch runs stable.
    return 0;
}
