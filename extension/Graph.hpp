
#ifndef Graph_hpp
#define Graph_hpp

#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include "string"
#include <vector>
#include <list>
#include <fstream>
#include <queue>
#include <algorithm>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "BitMap.hpp"
#include "HashSet.hpp"
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <mutex>
struct point{
    int vertex;
    float value;
//    int type;
};


struct k_index{
    int c;
    int f;
    int vertex;
};

struct k_index_core{
    int mu;
    int c;
    int f;
    int vertex;
};


using namespace std;
class Graph{
    string str;//图数据
    uint64_t n,m,edge_m;
    int k;
    int max_degree;
    // int *max_degree_num;
    int *degree;//每个点邻居数目
    uint32_t *pstart;
    uint32_t *out_pstart,*in_pstart; //offset of neighbors of nodes 节点相邻点偏移量
    unsigned int *out_edges,*in_edges; //adjacent ids of edges 相邻边
    unsigned int *edges;
    int *edge_type;//0:单出边  1:单入边 2:双边
    uint32_t *reverse, *reverse_out; //the position of reverse edge in edges 反向边在边中的位置
    // uint32_t *num_c, *num_f;
    uint32_t *num_out_c, *num_out_f;
//    float ***core_order_c, ***core_order_f, **neighbor_order_c, **neighbor_order_f;
    point *neighbor_order_c2, *neighbor_order_f2;
//    point **core_order_c2, **core_order_f2;
    k_index *neighbor_order_index;
//    int *neighbor_order_index_num, *core_order_index_num;
    k_index_core *core_order_index;
    // float *hash_c;
    // int *hash_c_pos;
//    int *hash_c_pos_cluster;
    int *order_c_;
    
    // bool *reverse_out_bool;
    // int *all_core;

    int* pa_;
    BitMap* core_bm_;
    BitMap* core_bm_judge_core;
//    int* core_bm_;
    vector<pair<int,int>> result_non_core_;
    // bool *query_visited;
    // int *hubs;
    int hubs_num;
    int *all_nei, *nei;
    // int *all_cluster_vertices, *all_v_in_cluster, *all_cluster_start, *core;
    // int *visit_all;

    
    uint32_t *core_num_pos;
    vector<vector<uint32_t>> core_order_index_num;
    uint32_t *neighbor_num_pos;
    vector<vector<uint32_t>> neighbor_order_index_num;

    int *all_c_num;
    
public:
    Graph(const char *_dir, int k);
    ~Graph();
    void readGraph();
    void creatIndex();
    
    void query_new_index(float parameter_c,float parameter_f, int parameterNum);//新的index方法
    void query_na(float parameter_c,float parameter_f, int parameterNum);//旧的方法
    
    void output(float parameter_c,float parameter_f, int parameterNum);
    void evaluateNeighborProxy(float parameter_c, float parameter_f,
                               double &precision, double &recall,
                               double &f1, double &rmse,
                               uint64_t &tp, uint64_t &fp, uint64_t &fn);
    void evaluateCoreNeighborCombinedProxy(float parameter_c, float parameter_f, int parameterNum,
                                           double &core_precision, double &core_recall, double &core_f1, double &core_rmse,
                                           double &neighbor_precision, double &neighbor_recall,
                                           double &neighbor_f1, double &neighbor_rmse,
                                           double &combined_precision, double &combined_recall,
                                           double &combined_f1);
    
private:
    void getDegree();
    void getTti();
    
    int intersection_c(unsigned int *A, unsigned int *B, uint64_t a1,uint64_t a2,uint64_t b1,uint64_t b2, int u);
    uint64_t binary_search(const unsigned int *array, uint64_t b, uint64_t e, int val) ;
    void initReverse();
    void initReverse_out();
    void getNeighborOrder();
    void getNeighborOrder_k();
    // void getCoreOrder4();
    void getCoreOrder4_k();
    void getAllNei(float parameter_c, float parameter_f, int v, int *all_nei, int &all_nei_num);
    uint64_t BinarySearch(int *a, int value, int n);
    int BinarySearch2(int *a, int value, int b, int e);
    int BinarySearch3(vector<int> a, int value);
    uint64_t BinarySearch4(unsigned int *a,unsigned int value, uint64_t b, uint64_t e);
    int BinarySearch4_(unsigned int *a,unsigned int value, unsigned int b, unsigned int e);
    int find_root(int u);

};
#endif /* Graph_hpp */




  



