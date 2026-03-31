#include <iostream>
#include <string>
#include <ctime>
#include "Graph.hpp"

using namespace std;

int main(int argc, const char *argv[]) {
    if (argc < 6) {
        cerr << "Usage: " << argv[0] << " <graph_dir> <k> <eps_c> <eps_f> <mu>" << endl;
        return 1;
    }

    const char *graph_dir = argv[1];
    const int k = atoi(argv[2]);
    float eps_c = atof(argv[3]);
    float eps_f = atof(argv[4]);
    int mu = atoi(argv[5]);

    Graph *graph = new Graph(graph_dir, k);

    clock_t start_time = clock();
    graph->readGraph();
    clock_t end_time = clock();
    cout << "Read graph: " << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;

    start_time = clock();
    graph->creatIndex();
    end_time = clock();
    cout << "Build index: " << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;

    start_time = clock();
    graph->query_new_index(eps_c, eps_f, mu);
    end_time = clock();
    cout << "Query: " << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;

    graph->output(eps_c, eps_f, mu);
    cout << "Results saved to output/" << endl;

    return 0;
}
