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
#include <unordered_map>
#include <unordered_set>
#include "BitMap.hpp"
#include "HashSet.hpp"
struct point{
    int vertex;
    float value;
    int type;
};

//struct update_v{
//    int vertex;
//    int type;
//};

using namespace std;
class Graph{
private:
    string str;//图数据
    uint64_t n,m,edge_m;
    int max_degree;
    int *max_degree_num;
    int *degree;//每个点邻居数目
    uint32_t *pstart;
    uint32_t *out_pstart,*in_pstart; //offset of neighbors of nodes 节点相邻点偏移量
    unsigned int *out_edges,*in_edges; //adjacent ids of edges 相邻边
    unsigned int *edges;
    int *edge_type;//0:单出边  1:单入边 2:双边
    uint32_t *reverse, *reverse_out; //the position of reverse edge in edges 反向边在边中的位置
    uint32_t *num_c, *num_f, *num_out_c, *num_out_f;
//    float ***core_order_c, ***core_order_f, **neighbor_order_c, **neighbor_order_f;
    point *neighbor_order_c2, *neighbor_order_f2;
    bool *reverse_out_bool;

    int* pa_;
    BitMap* core_bm_;
//    int* core_bm_;
    vector<pair<int,int>> result_non_core_;
    bool *query_visited;
    int *hubs;
    int hubs_num;
    int *all_nei, *all_cluster_vertices, *nei, *all_v_in_cluster, *all_cluster_start, *all_core, *core;

    vector<multimap<float, int, greater<float>>> nbr_mp_c;
    vector<multimap<float, int, greater<float>>> nbr_mp_f;
    vector<int> update_degree, update_max_degree_num, update_in_num, update_out_num;
    vector<vector<int>> update_in_edges, update_out_edges;
    vector<vector<float>> nbr_order_old_c, nbr_order_old_f;
public:
    Graph(const char *_dir);
    ~Graph();
    void readGraph();
    void creatIndex();
    void readIndex();
    int readGraph_update();
    void readIndex_update();
    void query(float parameter1,float parameter2, int parameterNum);//新的方法+hub
    void query4(float parameter1,float parameter2, int parameterNum);//新的方法+hub
    void insert(int u, int v);
    void remove(int u, int v);
    void insert_na(int u, int v);
    void remove_na(int u, int v);
    void update_write();
    void output(float parameter1,float parameter2, int parameterNum);
private:
    void getDegree();
    void getTti();
    void getTti_na();
    void getFlowTti();
    int intersection_f(unsigned int *A, unsigned int *B, uint64_t a1, uint64_t a2, uint64_t b1, uint64_t b2);
    int intersection_c(unsigned int *A, unsigned int *B, uint64_t a1,uint64_t a2,uint64_t b1,uint64_t b2, int u);
    uint64_t binary_search(const unsigned int *array, uint64_t b, uint64_t e, int val) ;
    void initReverse();
    void initReverse_out();
    void getNeighborOrder();
    void getNeighborOrder_na();
    vector<int> get_f_and_c_num(int a,int b, int c);
    void writeIndex();
    void getAllNei(float parameter1, float parameter2, int v, int *all_nei, int &all_nei_num);
    void getAllNei2(float parameter1, float parameter2, int v, int *all_nei, int &all_nei_num);
    int getAllNei3(float parameter1, float parameter2, int v);
    uint64_t BinarySearch(int *a, int value, int n);
    int BinarySearch2(int *a, int value, int b, int e);
    int BinarySearch3(vector<int> a, int value);
    uint64_t BinarySearch4(unsigned int *a,unsigned int value, uint64_t b, uint64_t e);
    int BinarySearch4_(unsigned int *a,unsigned int value, unsigned int b, unsigned int e);
    int find_root(int u);
    
    void get_add_c_and_f(unordered_map<int,int> &add_c, unordered_map<int,int> &add_f, int u, int v, int &all_c, int &all_f);
    void update_nei1(unordered_map<int,int> &add_c, unordered_map<int,int> &add_f, int all_c, int all_f, int u, int v);//有邻居增加
    void update_nei2(unordered_map<int,int> &add_c, unordered_map<int,int> &add_f, int all_c, int all_f, int u, int v);//无邻居增加
    void update_nei_order_c(int v, int w, float old_sim, float new_sim);
    void update_nei_order_f(int v, int w, float old_sim, float new_sim);
    
    void delete_update_nei1(unordered_map<int,int> &add_c, unordered_map<int,int> &add_f, int all_c, int all_f, int u, int v);//有邻居减少
    void delete_update_nei2(unordered_map<int,int> &add_c, unordered_map<int,int> &add_f, int all_c, int all_f, int u, int v);//无邻居减少
    
    void update_nei1_4(int u, int v);//有邻居增加
    void update_nei2_4(int u, int v);//无邻居增加

    void delete_update_nei1_4(int u, int v);//有邻居减少
    void delete_update_nei2_4(int u, int v);//无邻居减少

    void get_add_c4(int u, int v, int &all_f);
    void get_add_f4(int u, int v, int &all_f);
};
#endif /* Graph_hpp */




  
