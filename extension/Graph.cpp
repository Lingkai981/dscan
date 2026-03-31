//
//  Graph.cpp
//  index_new
//
//  Created by 孟令凯 on 2021/12/17.
//
#include "Graph.hpp"
#include <algorithm>
#include "math.h"

Graph::Graph(const char *_dir, int k){
    this->str = string(_dir);
    n = m = edge_m = max_degree = hubs_num = 0;
    this->k = k;
    hubs_num = 0;
    degree = NULL;
    pstart = NULL;
    out_pstart = NULL;
    in_pstart = NULL;
    edges= NULL;
    out_edges= NULL;
    in_edges= NULL;
    reverse= NULL;
    reverse_out= NULL;
    // num_c= NULL;
    // num_f= NULL;
    num_out_c= NULL;
    num_out_f= NULL;
    neighbor_order_c2= NULL;
    neighbor_order_f2= NULL;
    // max_degree_num = NULL;
    edge_type = NULL;
    
    pa_ = NULL;
    core_bm_ = NULL;
    core_bm_judge_core = NULL;
    
    // query_visited = NULL;
    // hubs = NULL;
    all_nei = NULL;
    // all_cluster_vertices = NULL;
    nei = NULL;
    // all_v_in_cluster = NULL;
    // all_cluster_start = NULL;
    // core = NULL;
    // all_core = NULL;
    neighbor_order_index = NULL;
    core_order_index = NULL;
    // hash_c = NULL;
    // hash_c_pos = NULL;
//    hash_c_pos_cluster = NULL;
    core_num_pos = NULL;
    neighbor_num_pos = NULL;
    // all_core = NULL;
    // visit_all = NULL;
    order_c_ = NULL;
    all_c_num = NULL;
}
Graph::~Graph(){

    if(all_c_num!=NULL){
        delete [] all_c_num;
        all_c_num = NULL;
    }

    if(pa_!=NULL){
        delete [] pa_; pa_ = NULL;
    }
    // if(core!=NULL){
    //     delete [] core; core = NULL;
    // }
    // if(all_core!=NULL){
    //     delete [] all_core; all_core = NULL;
    // }
    if(degree!=NULL){
        delete [] degree;
        degree = NULL;
    }
    if(pstart!=NULL){
        delete [] pstart;
        pstart = NULL;
    }
    if(out_pstart!=NULL){
        delete [] out_pstart;
        out_pstart = NULL;
    }
    if(in_pstart!=NULL){
        delete [] in_pstart;
        in_pstart = NULL;
    }
    if(edge_type!=NULL){
        delete [] edge_type;
        edge_type = NULL;
    }
    if(edges!=NULL){
        delete [] edges;
        edges = NULL;
    }
    if(out_edges!=NULL){
        delete [] out_edges;
        out_edges = NULL;
    }
    if(in_edges!=NULL){
        delete [] in_edges;
        in_edges = NULL;
    }
    if(reverse!=NULL){
        delete [] reverse;
        reverse = NULL;
    }
    if(reverse_out!=NULL){
        delete [] reverse_out;
        reverse_out = NULL;
    }
    // if(num_c!=NULL){
    //     delete [] num_c;
    //     num_c = NULL;
    // }
    // if(num_f!=NULL){
    //     delete [] num_f;
    //     num_f = NULL;
    // }
    if(num_out_c!=NULL){
        delete [] num_out_c;
        num_out_c = NULL;
    }
    if(num_out_f!=NULL){
        delete [] num_out_f;
        num_out_f = NULL;
    }
    // if(reverse_out_bool!=NULL){
    //     delete [] reverse_out_bool;
    //     reverse_out_bool = NULL;
    // }
    // if(query_visited!=NULL){
    //     delete [] query_visited;
    //     query_visited = NULL;
    // }
    // if(hubs!=NULL){
    //     delete [] hubs;
    //     hubs = NULL;
    // }
   
    if(neighbor_order_f2!=NULL){
//        delete[] neighbor_order_f[0];
//        for (int i = 0; i < edge_m; i++) {
//            neighbor_order_f[i] = NULL;
//        }
        delete[] neighbor_order_f2;
        neighbor_order_f2 = NULL;
    }
    // if(max_degree_num!=NULL){
    //     delete [] max_degree_num;
    //     max_degree_num = NULL;
    // }
    
    if(neighbor_order_c2!=NULL){
//        delete[] neighbor_order_c[0];
//        for (int i = 0; i < edge_m; i++) {
//            neighbor_order_c[i] = NULL;
//        }
        delete[] neighbor_order_c2;
        neighbor_order_c2 = NULL;
    }
    
    if(all_nei!=NULL){
        delete [] all_nei; all_nei = NULL;
    }
    if(nei!=NULL){
        delete [] nei; nei = NULL;
    }
    // if(all_cluster_vertices!=NULL){
    //     delete [] all_cluster_vertices; all_cluster_vertices = NULL;
    // }
    // if(all_v_in_cluster!=NULL){
    //     delete [] all_v_in_cluster; all_v_in_cluster = NULL;
    // }
    // if(all_cluster_start!=NULL){
    //     delete [] all_cluster_start; all_cluster_start = NULL;
    // }
    if(neighbor_order_index!=NULL){
        delete [] neighbor_order_index; neighbor_order_index = NULL;
    }
    if(core_order_index!=NULL){
        delete [] core_order_index; core_order_index = NULL;
    }
    // if(hash_c!=NULL){
    //     delete [] hash_c; hash_c = NULL;
    // }
    // if(hash_c_pos!=NULL){
    //     delete [] hash_c_pos; hash_c_pos = NULL;
    // }
//    if(hash_c_pos_cluster!=NULL){
//        delete [] hash_c_pos_cluster; hash_c_pos_cluster = NULL;
//    }
    if(core_num_pos!=NULL){
        delete [] core_num_pos; core_num_pos = NULL;
    }
    if(neighbor_num_pos!=NULL){
        delete [] neighbor_num_pos; neighbor_num_pos = NULL;
    }
    // if(all_core!=NULL){
    //     delete [] all_core; all_core = NULL;
    // }
    // if(visit_all!=NULL){
    //     delete [] visit_all; visit_all = NULL;
    // }
    if(order_c_!=NULL){
        delete [] order_c_; order_c_ = NULL;
    }
}

void Graph::readGraph(){
    ifstream infile;   //输入流
    
    infile.open(str+"/degree.txt", ios::in);
    if (!infile.is_open()){
        cout<<"Open degree file failure"<<endl;
        exit(0);
    }
    infile>>n>>m;

    if(core_bm_ == NULL){
        // cout<<13.33<<endl;
        // cout<<n<<endl;
        core_bm_ = new BitMap(n);
    } 
    // cout<<13.4<<endl;
    if(core_bm_judge_core == NULL) core_bm_judge_core = new BitMap(n);
    
    if(out_pstart == NULL) out_pstart = new uint32_t[n+1];
    if(in_pstart == NULL) in_pstart = new uint32_t[n+1];
    if(out_edges == NULL) out_edges = new unsigned int[(long)m];
    if(in_edges == NULL) in_edges = new unsigned int[(long)m];
    
    out_pstart[0] = 0;
    in_pstart[0] = 0;
    
    int outD, inD, starti = 0;
    
    while (!infile.eof())            // 若未到文件结束一直循环
    {
        infile >> outD >> inD;
        out_pstart[starti+1] = outD + out_pstart[starti];
        in_pstart[starti+1] = inD + in_pstart[starti];
        starti++;
    }
    infile.close();
    
    infile.open(str+"/out_edges.txt", ios::in);
    if (!infile.is_open()){
        cout<<"Open out_edges file failure"<<endl;
        exit(0);
    }
    int mm_;
    infile >> mm_;
    uint64_t outi = 0;
    while (!infile.eof())            // 若未到文件结束一直循环
    {
        infile >> out_edges[outi];
        outi++;
    }
    infile.close();
    
    infile.open(str+"/in_edges.txt", ios::in);
    
    if (!infile.is_open()){
        cout<<"Open in_edges file failure"<<endl;
        exit(0);
    }
    uint64_t ini = 0;
    infile >> mm_;
    while (!infile.eof())            // 若未到文件结束一直循环
    {
        infile >> in_edges[ini];
        ini++;
    }
    infile.close();
    
    getDegree();
    return;


    int *is_chosed = new int[n];
    int *change = new int[n];

    for(int i = 0;i<n;i++) is_chosed[i] = 0;

    std::srand(std::time(0)); // 使用当前时间作为随机数生成器的种子
    int random_number = std::rand() % 100; // 生成0到99之间的随机数

    int real_v = 0;
    for(int i = 0;i<n;i++){
        if(rand() % 100 > 50){
            is_chosed[i] = 1;
            change[i] = real_v++;
        }
    }

    unsigned int *out_edges_ = new unsigned int[(long)m];
    unsigned int *in_edges_ = new unsigned int[(long)m];

    unsigned int *out_degree_ = new unsigned int[n];
    unsigned int *in_degree_ = new unsigned int[n];

    unsigned int in_edge_i = 0;
    unsigned int in_degree_i = 0;
    unsigned int out_edge_i = 0;
    unsigned int out_degree_i = 0;

    for (int i = 0;i<n;i++){
        if(is_chosed[i]){
            int in_num = 0, out_num = 0;

            for(unsigned int j = out_pstart[i]; j<out_pstart[i+1];j++){
                if(is_chosed[out_edges[j]] && rand() % 100 > 10){
                    out_num++;
                    out_edges_[out_edge_i++] = change[out_edges[j]];
                }
            }

            for(unsigned int j = in_pstart[i]; j<in_pstart[i+1];j++){
                if(is_chosed[in_edges[j]]&& rand() % 100 > 10){
                    in_num++;
                    in_edges_[in_edge_i++] = change[in_edges[j]];
                }
            }

            out_degree_[out_degree_i++] = out_num;
            in_degree_[in_degree_i++] = in_num;
        }
    }

    std::ofstream outFile("/home/data/mlk/data/webbase_sample1/webbase_sample1/degree.txt"); // 创建并打开文件
    
    if (!outFile.is_open()) { // 检查文件是否成功打开
        std::cerr << "Failed to open the file!" << std::endl;
        return ;
    }

    outFile << real_v << " "<<max(in_edge_i, out_edge_i)<<"\n";

    for(int i = 0;i< real_v;i++){

        outFile <<out_degree_[i]<<" "<<in_degree_[i]<<endl;

    }

    std::ofstream outFile2("/home/data/mlk/data/webbase_sample1/webbase_sample1/out_edges.txt"); // 创建并打开文件
    
    if (!outFile2.is_open()) { // 检查文件是否成功打开
        std::cerr << "Failed to open the file!" << std::endl;
        return ;
    }

    outFile2 << real_v <<"\n";

    for(unsigned int i = 0;i< out_edge_i;i++){

        outFile2 <<out_edges_[i]<<endl;

    }

    std::ofstream outFile3("/home/data/mlk/data/webbase_sample1/webbase_sample1/in_edges.txt"); // 创建并打开文件
    
    if (!outFile3.is_open()) { // 检查文件是否成功打开
        std::cerr << "Failed to open the file!" << std::endl;
        return ;
    }

    outFile3 << real_v <<"\n";

    for(unsigned int i = 0;i< in_edge_i;i++){

        outFile3 <<in_edges_[i]<<endl;

    }

}

void Graph::creatIndex(){
    getTti();
    
    if(in_pstart!=NULL){
        delete [] in_pstart;
        in_pstart = NULL;
    }

    if(in_edges!=NULL){
        delete [] in_edges;
        in_edges = NULL;
    }
        
    getNeighborOrder();
    
    if(out_edges!=NULL){
        delete [] out_edges;
        out_edges = NULL;
    }
    if(out_pstart!=NULL){
        delete [] out_pstart;
        out_pstart = NULL;
    }
    if(reverse!=NULL){
        delete [] reverse;
        reverse = NULL;
    }
    if(num_out_c!=NULL){
        delete [] num_out_c;
        num_out_c = NULL;
    }
    if(num_out_f!=NULL){
        delete [] num_out_f;
        num_out_f = NULL;
    }
    // if(reverse_out_bool!=NULL){
    //     delete [] reverse_out_bool;
    //     reverse_out_bool = NULL;
    // }
    if(edges!=NULL){
        delete [] edges;
        edges = NULL;
    }
    if(reverse_out!=NULL){
        delete [] reverse_out;
        reverse_out = NULL;
    }
    if(edge_type!=NULL){
        delete [] edge_type;
        edge_type = NULL;
    }
//    getCoreOrder4();
    getCoreOrder4_k();
}

void Graph::getDegree(){
    if(pstart == NULL) pstart = new uint32_t[n+1];
    if(degree == NULL) degree = new int[n];
    pstart[0] = 0;
    if(edges == NULL)  edges = new unsigned int[(long)2*m];
    if(edge_type == NULL)  edge_type = new int[(long)2*m];
    long edgesi = 0;
    for(int i = 0;i<n;i++){
        int l1 = out_pstart[i+1] - out_pstart[i];
        int l2 = in_pstart[i+1] - in_pstart[i];
        int i1 = 0, i2 = 0, num = 0;
        while(i1<l1 && i2<l2){
            if(out_edges[out_pstart[i] + i1] == in_edges[in_pstart[i] + i2]){
                edges[edgesi] = out_edges[out_pstart[i] + i1];
                edge_type[edgesi] = 2;
                edgesi++;
                i1++;
                i2++;
                num++;
            }else if(out_edges[out_pstart[i] + i1] < in_edges[in_pstart[i] + i2]){
                edges[edgesi] = out_edges[out_pstart[i] + i1];
                edge_type[edgesi] = 0;
                edgesi++;
                i1++;
                num++;
            }else{
                edges[edgesi] = in_edges[in_pstart[i] + i2];
                edge_type[edgesi] = 1;
                edgesi++;
                i2++;
                num++;
            }
        }
        while(i1<l1){
            edges[edgesi] = out_edges[out_pstart[i] + i1];
            edge_type[edgesi] = 0;
            edgesi++;
            i1++;
            num++;
        }
        while(i2<l2){
            edges[edgesi] = in_edges[in_pstart[i] + i2];
            edge_type[edgesi] = 1;
            edgesi++;
            i2++;
            num++;
        }
        if(num>max_degree) max_degree = num;
        
        degree[i] = num;
        edge_m = edge_m + num;
        pstart[i+1] = pstart[i] + num;
    }

    
    if(reverse == NULL) reverse = new uint32_t[(long)edge_m];
    if(reverse_out == NULL) reverse_out = new uint32_t[(long)(m+1)];
    // reverse_out_bool = new bool[(long)(m+1)];
//    num_c = new unsigned int[(long)edge_m];
//    num_f = new unsigned int[(long)edge_m];
    if(num_out_c == NULL) num_out_c = new uint32_t[(long)(m+1)];
    if(num_out_f == NULL) num_out_f = new uint32_t[(long)(m+1)];
    for(long i = 0;i<m;i++) {
        num_out_c[i] = 0;
        num_out_f[i] = 0;
        reverse_out[i] = m;
        // reverse_out_bool[i] = true;
    }
    num_out_c[(long)m] = 0;
    num_out_f[(long)m] = 0;
    // reverse_out_bool[(long)m] = true;
    initReverse();
    initReverse_out();
}

int Graph::intersection_c(unsigned int *A, unsigned int *B,uint64_t a1,uint64_t a2,uint64_t b1,uint64_t b2, int u){
    int outi = 0;
    uint64_t i = a1, j = b1;
    while(i<a2 && j<b2){
        if(A[i] == B[j]){
            if(u<A[i]){
                int v = in_edges[i];
                num_out_c[binary_search(out_edges,out_pstart[v],out_pstart[v+1],u)]++;
                num_out_c[j]++;
                outi++;
            }
            i++;
            j++;
        }else if(A[i]<B[j]){
            i++;
        }else{
            j++;
        }
    }
    return outi;
    
}

uint64_t Graph::binary_search(const unsigned int *array, uint64_t b, uint64_t e, int val) {
#ifdef _DEBUG_
    if(e < b) printf("??? WA1 in binary_search\n");
#endif
    -- e;
    if(array[e] < val) return e+1;
    while(b < e) {
        uint64_t mid = b + (e-b)/2;
        if(array[mid] >= val) e = mid;
        else b = mid+1;
    }
#ifdef _DEBUG_
    if(array[e] < val) printf("??? WA2 in binary_search\n");
#endif
    return e;
}

uint64_t Graph::BinarySearch(int *a, int value, int n){
    int low, high, mid;
    low = 0;
    high = n-1;
    while(low<=high)
    {
        mid = (low+high)/2;
        if(a[mid]==value)
            return 1;
        if(a[mid]>value)
            high = mid-1;
        if(a[mid]<value)
            low = mid+1;
    }
    return 0;
}

int Graph::BinarySearch2(int *a, int value, int b, int e){
    int low, high, mid;
    low = b;
    high = e-1;
    while(low<=high)
    {
        mid = (low+high)/2;
        if(a[mid]==value)
            return 1;
        if(a[mid]>value)
            high = mid-1;
        if(a[mid]<value)
            low = mid+1;
    }
    return 0;
}
uint64_t Graph::BinarySearch4(unsigned int *a,unsigned int value, uint64_t b, uint64_t e){
    long low, high, mid;
    low = b;
    high = e-1;
    while(low<=high)
    {
        mid = (low+high)/2;
        if(a[mid]==value)
            return (uint64_t)mid;
        if(a[mid]>value)
            high = mid-1;
        if(a[mid]<value)
            low = mid+1;
    }
    return m;
}

int Graph::BinarySearch3(vector<int> a, int value){
    int low, high, mid;
    low = 0;
    high = (int)a.size()-1;
    while(low<=high)
    {
        mid = (low+high)/2;
        if(a[mid]==value)
            return 1;
        if(a[mid]>value)
            high = mid-1;
        if(a[mid]<value)
            low = mid+1;
    }
    return 0;
}

void Graph::initReverse(){
    for(unsigned int i = 0;i<n;i++){
        for(uint64_t j = pstart[i]; j<pstart[i+1];j++){
            unsigned int v = edges[j];
            if(v<i) continue;
            uint64_t reverse_edge_id = binary_search(edges, pstart[v], pstart[v+1], i);
            reverse[j] = reverse_edge_id;
            reverse[reverse_edge_id] = j;
        }
    }
}

void Graph::initReverse_out(){
    for(unsigned int i = 0;i<n;i++){
        for(uint64_t j = out_pstart[i]; j<out_pstart[i+1];j++){
            unsigned int v = out_edges[j];
            if(v<i) continue;
            uint64_t reverse_edge_id = BinarySearch4(out_edges, i, out_pstart[v], out_pstart[v+1]);
            if(reverse_edge_id < m){
                reverse_out[j] = reverse_edge_id;
                reverse_out[reverse_edge_id] = j;
            }
            
        }
    }
}

void Graph::getTti(){
    long *visit_out = new long[n];
    memset(visit_out, -1, sizeof(long)*n);
    for(int i = 0; i < n; i++){
        for(unsigned int j = out_pstart[i]; j<out_pstart[i+1]; j++){
            visit_out[out_edges[j]] = j;
        }
        for(unsigned int j = out_pstart[i]; j<out_pstart[i+1]; j++){
            unsigned int v = out_edges[j];
            if(((out_pstart[v+1] - out_pstart[v]) < (out_pstart[i+1] - out_pstart[i])) || (((out_pstart[v+1] - out_pstart[v]) == (out_pstart[i+1] - out_pstart[i])) && (i<v))){
                int double_ = 1;
                if(reverse_out[j] != m) double_++;
                for(unsigned int k = out_pstart[v]; k<out_pstart[v+1]; k++){

                    unsigned int w = out_edges[k];
                    if(w == i) continue;
                    if(visit_out[w]>=0){
                        num_out_f[k]+=double_;
                        num_out_f[j]+=double_;
                        num_out_f[visit_out[w]]+=double_;
                    }
                }
            }
            if(i < v){
                int num = intersection_c(in_edges, out_edges, in_pstart[i],  in_pstart[i+1], out_pstart[v], out_pstart[v+1], i);
                num_out_c[j] = num_out_c[j] + num;
            }
        }
        for(unsigned int j = in_pstart[i]; j<in_pstart[i+1]; j++){

            unsigned int v = in_edges[j];
            if(visit_out[v]>=0) continue;
            if(((out_pstart[v+1] - out_pstart[v]) < (out_pstart[i+1] - out_pstart[i])) || (((out_pstart[v+1] - out_pstart[v]) == (out_pstart[i+1] - out_pstart[i])) && (i<v))){
                for(unsigned int k = out_pstart[v]; k<out_pstart[v+1]; k++){
                    unsigned int w = out_edges[k];
                    if(w == i) continue;
                    if(visit_out[w]>=0){
                        num_out_f[k]++;
                        num_out_f[binary_search(out_edges,out_pstart[v],out_pstart[v+1],i)]++;
                        num_out_f[visit_out[w]]++;
                    }
                }
            }
        }

        for(unsigned int j = out_pstart[i]; j<out_pstart[i+1]; j++){
            visit_out[out_edges[j]] = -1;
        }
    }

    delete [] visit_out;
    visit_out = NULL;
}



bool comp(float* s1,float* s2){//相似度从大到小
    
    return s1[1]>s2[1];
}


bool comp2(float* s1,float* s2){//相邻点从小到大
    
    return s1[0]<s2[0];
}

bool comp3(point s1,point s2){//相似度从大到小
    
    return s1.value>s2.value;
}



bool comp_nei_order_index(k_index s1, k_index s2){//相似度从大到小
    if(s1.f == s2.f){
        return s1.c>s2.c;
    }
    return s1.f>s2.f;
}
bool comp_core_order_index(k_index_core s1, k_index_core s2){//相似度从大到小
    if(s1.mu == s2.mu){
        if(s1.f == s2.f){
            return s1.c>s2.c;
        }
        return s1.f>s2.f;
    }
    return s1.mu<s2.mu;
}

bool comp4(out_vertex2 s1, out_vertex2 s2){//相似度从大到小
    
    return s1.value>s2.value;
}



void Graph::getNeighborOrder(){
    neighbor_order_index_num.resize(n+2);//, vector<uint32_t>(k+2));
    vector<vector<int>> neighbor_order_index_num2(k+2, vector<int>(k+2, 0));
    if(neighbor_order_c2 == NULL) neighbor_order_c2 = new point[edge_m];
    if(neighbor_order_f2 == NULL) neighbor_order_f2 = new point[edge_m];
    if(neighbor_order_index == NULL) neighbor_order_index = new k_index[edge_m];
    if(neighbor_num_pos == NULL) neighbor_num_pos = new uint32_t[n];
    int cycle_num = 4, flow_num = 12;
    for(int i = 0;i<n;i++){
        for(uint32_t init_j = pstart[i]; init_j < pstart[i+1]; ++init_j){
            neighbor_order_c2[init_j].vertex = edges[init_j];
            neighbor_order_c2[init_j].value = 0.0f;
            neighbor_order_f2[init_j].vertex = edges[init_j];
            neighbor_order_f2[init_j].value = 0.0f;
            neighbor_order_index[init_j].vertex = edges[init_j];
            neighbor_order_index[init_j].c = 0;
            neighbor_order_index[init_j].f = 0;
        }
        uint64_t j, out_j;
        for(j = pstart[i], out_j = out_pstart[i];j<pstart[i+1];){
            int v = edges[j], out_v = out_edges[out_j];
            if(v<i){
                if(v == out_v){
                    if(reverse_out[out_j] == m){
                        if(edge_type[j] == 2){
                            cycle_num = 4;
                            flow_num = 12;
                        }else{
                            cycle_num = 0;
                            flow_num = 4;
                        }

                        float c_value = (num_out_c[out_j] + num_out_c[reverse_out[out_j]] + cycle_num) / (2*(float)pow((long)((long)(degree[i]+1) * (long)(degree[v]+1)),0.5));
                        float f_value = (num_out_f[out_j] + num_out_f[reverse_out[out_j]] + flow_num) / (6*(float)pow((long)((long)(degree[i]+1) * (long)(degree[v]+1)),0.5));

//                        c_value = (float)((int)((c_value+0.0005)*1000))/1000;
//                        f_value = (float)((int)((f_value+0.0005)*1000))/1000;
if(c_value>1){
    c_value = 1;
}
if(f_value>1){
    f_value = 1;
}



                        neighbor_order_c2[reverse[j]].value = neighbor_order_c2[j].value = c_value;
                        neighbor_order_index[j].vertex = neighbor_order_c2[j].vertex = v;
                        neighbor_order_index[reverse[j]].vertex = neighbor_order_c2[reverse[j]].vertex = i;
//                        neighbor_order_c2[j].type = edge_type[j];
//                        neighbor_order_c2[reverse[j]].type = edge_type[reverse[j]];

                        neighbor_order_f2[reverse[j]].value = neighbor_order_f2[j].value = f_value;
                        neighbor_order_f2[j].vertex = v;
                        neighbor_order_f2[reverse[j]].vertex = i;
//                        neighbor_order_f2[j].type = edge_type[j];
//                        neighbor_order_f2[reverse[j]].type = edge_type[reverse[j]];

                        neighbor_order_index[j].c = neighbor_order_index[reverse[j]].c = (int)(c_value*k);
                        neighbor_order_index[j].f = neighbor_order_index[reverse[j]].f = (int)(f_value*k);

                    }
                    j++;
                    out_j++;
                }else{
                    j++;
                }
                continue;
            }
            if(v == out_v){
                if(edge_type[j] == 2){
                    cycle_num = 4;
                    flow_num = 12;
                }else{
                    cycle_num = 0;
                    flow_num = 4;
                }

                float c_value = (num_out_c[out_j]+ num_out_c[reverse_out[out_j]]+ cycle_num) / (2*(float)pow((long)((long)(degree[i]+1) * (long)(degree[v]+1)),0.5));
                float f_value = (num_out_f[out_j]+ num_out_f[reverse_out[out_j]]+ flow_num) / (6*(float)pow((long)((long)(degree[i]+1) * (long)(degree[v]+1)),0.5));

if(c_value>1){
    c_value = 1;
}
if(f_value>1){
    f_value = 1;
}
//                c_value = (float)((int)((c_value+0.0005)*1000))/1000;
//                f_value = (float)((int)((f_value+0.0005)*1000))/1000;
// if(c_value>1){
//     cout<<"c_value:"<<c_value<<endl;
// }
// if(f_value>1){
//     cout<<"f_value:"<<f_value<<endl;
// }

                neighbor_order_c2[reverse[j]].value = neighbor_order_c2[j].value = c_value;
                neighbor_order_index[j].vertex = neighbor_order_c2[j].vertex = v;
                neighbor_order_index[reverse[j]].vertex = neighbor_order_c2[reverse[j]].vertex = i;
//                neighbor_order_c2[j].type = edge_type[j];
//                neighbor_order_c2[reverse[j]].type = edge_type[reverse[j]];

                neighbor_order_f2[reverse[j]].value = neighbor_order_f2[j].value = f_value;
                neighbor_order_f2[j].vertex = v;
                neighbor_order_f2[reverse[j]].vertex = i;
//                neighbor_order_f2[j].type = edge_type[j];
//                neighbor_order_f2[reverse[j]].type = edge_type[reverse[j]];

                neighbor_order_index[j].c = neighbor_order_index[reverse[j]].c = (int)(c_value*k);
                neighbor_order_index[j].f = neighbor_order_index[reverse[j]].f = (int)(f_value*k);

                j++;
                out_j++;
            }else{
                j++;
            }
        }
    }

    for(int i = 0;i<n;i++){
        sort(neighbor_order_c2+pstart[i], neighbor_order_c2+pstart[i+1],comp3);
        sort(neighbor_order_f2+pstart[i], neighbor_order_f2+pstart[i+1],comp3);
        sort(neighbor_order_index+pstart[i], neighbor_order_index+pstart[i+1],comp_nei_order_index);
    }

    int n_i = 0, now_f = k+1;
    for(int i = 0;i<n;i++){
        n_i = 0;
        now_f = k+1;
        for(uint32_t j = pstart[i];j<pstart[i+1];j++){
            if(neighbor_order_index[j].f != now_f){
                neighbor_order_index_num[i].push_back(j);
                n_i++;
                now_f = neighbor_order_index[j].f;
            }
        }
        neighbor_order_index_num[i].push_back(pstart[i+1]);
        n_i++;
        neighbor_num_pos[i] = n_i;
    }
}

void Graph::getNeighborOrder_k(){
    neighbor_order_index_num.resize(n+2);//, vector<uint32_t>(k+2));
    vector<vector<int>> neighbor_order_index_num2(k+2, vector<int>(k+2, 0));
   neighbor_order_c2 = new point[edge_m];
   neighbor_order_f2 = new point[edge_m];
    neighbor_order_index = new k_index[edge_m];
    if(neighbor_num_pos == NULL) neighbor_num_pos = new uint32_t[n];
    int cycle_num = 4, flow_num = 12;
    for(int i = 0;i<n;i++){
        for(uint32_t init_j = pstart[i]; init_j < pstart[i+1]; ++init_j){
            neighbor_order_c2[init_j].vertex = edges[init_j];
            neighbor_order_c2[init_j].value = 0.0f;
            neighbor_order_f2[init_j].vertex = edges[init_j];
            neighbor_order_f2[init_j].value = 0.0f;
            neighbor_order_index[init_j].vertex = edges[init_j];
            neighbor_order_index[init_j].c = 0;
            neighbor_order_index[init_j].f = 0;
        }
        uint64_t j, out_j;
        for(j = pstart[i], out_j = out_pstart[i];j<pstart[i+1];){
            int v = edges[j], out_v = out_edges[out_j];
            if(v<i){
                if(v == out_v){
                    if(reverse_out[out_j] == m){
                        if(edge_type[j] == 2){
                            cycle_num = 4;
                            flow_num = 12;
                        }else{
                            cycle_num = 0;
                            flow_num = 4;
                        }
                        
                        float c_value = (num_out_c[out_j] + num_out_c[reverse_out[out_j]] + cycle_num) / (2*(float)pow((long)((long)(degree[i]+1) * (long)(degree[v]+1)),0.5));
                        float f_value = (num_out_f[out_j] + num_out_f[reverse_out[out_j]] + flow_num) / (6*(float)pow((long)((long)(degree[i]+1) * (long)(degree[v]+1)),0.5));
                        if(c_value>1) c_value = 1;
                        if(f_value>1) f_value = 1;



                        neighbor_order_c2[reverse[j]].value = neighbor_order_c2[j].value = c_value;
                        neighbor_order_index[j].vertex = neighbor_order_c2[j].vertex = v;
                        neighbor_order_index[reverse[j]].vertex = neighbor_order_c2[reverse[j]].vertex = i;

                        neighbor_order_f2[reverse[j]].value = neighbor_order_f2[j].value = f_value;
                        neighbor_order_f2[j].vertex = v;
                        neighbor_order_f2[reverse[j]].vertex = i;

                        neighbor_order_index[j].c = neighbor_order_index[reverse[j]].c = (int)(c_value*k);
                        neighbor_order_index[j].f = neighbor_order_index[reverse[j]].f = (int)(f_value*k);
                        
                    }
                    j++;
                    out_j++;
                }else{
                    j++;
                }
                continue;
            }
            if(v == out_v){
                if(edge_type[j] == 2){
                    cycle_num = 4;
                    flow_num = 12;
                }else{
                    cycle_num = 0;
                    flow_num = 4;
                }
                
                float c_value = (num_out_c[out_j]+ num_out_c[reverse_out[out_j]]+ cycle_num) / (2*(float)pow((long)((long)(degree[i]+1) * (long)(degree[v]+1)),0.5));
                float f_value = (num_out_f[out_j]+ num_out_f[reverse_out[out_j]]+ flow_num) / (6*(float)pow((long)((long)(degree[i]+1) * (long)(degree[v]+1)),0.5));
                if(c_value>1) c_value = 1;
                if(f_value>1) f_value = 1;
                
                neighbor_order_c2[reverse[j]].value = neighbor_order_c2[j].value = c_value;
                neighbor_order_index[j].vertex = neighbor_order_c2[j].vertex = v;
                neighbor_order_index[reverse[j]].vertex = neighbor_order_c2[reverse[j]].vertex = i;

                neighbor_order_f2[reverse[j]].value = neighbor_order_f2[j].value = f_value;
                neighbor_order_f2[j].vertex = v;
                neighbor_order_f2[reverse[j]].vertex = i;

                neighbor_order_index[j].c = neighbor_order_index[reverse[j]].c = (int)(c_value*k);
                neighbor_order_index[j].f = neighbor_order_index[reverse[j]].f = (int)(f_value*k);
                
                j++;
                out_j++;
            }else{
                j++;
            }
        }
    }

    
    
    for(int i = 0;i<n;i++){
        sort(neighbor_order_c2+pstart[i], neighbor_order_c2+pstart[i+1],comp3);
        sort(neighbor_order_f2+pstart[i], neighbor_order_f2+pstart[i+1],comp3);
        sort(neighbor_order_index+pstart[i], neighbor_order_index+pstart[i+1],comp_nei_order_index);
    }

    int n_i = 0, now_f = k+1;
    for(int i = 0;i<n;i++){
        n_i = 0;
        now_f = k+1;
        for(uint32_t j = pstart[i];j<pstart[i+1];j++){
            if(neighbor_order_index[j].f != now_f){
                neighbor_order_index_num[i].push_back(j);
                n_i++;
                now_f = neighbor_order_index[j].f;
            }
        }
        neighbor_order_index_num[i].push_back(pstart[i+1]);
        n_i++;
        neighbor_num_pos[i] = n_i;
    }
}

// void Graph::getNeighborOrder_k(){
//     neighbor_order_index_num.resize(n+2, vector<uint32_t>(k+2));
//     vector<vector<int>> neighbor_order_index_num2(k+2, vector<int>(k+2, 0));
// //    neighbor_order_c2 = new point[edge_m];
// //    neighbor_order_f2 = new point[edge_m];
//     if(neighbor_order_index == NULL) neighbor_order_index = new k_index[edge_m];
//     if(neighbor_num_pos == NULL) neighbor_num_pos = new uint32_t[n];
//     int cycle_num = 4, flow_num = 12;
//     for(int i = 0;i<n;i++){
//         uint64_t j, out_j;
        
//         for(j = pstart[i], out_j = out_pstart[i];j<pstart[i+1];){
//             int v = edges[j], out_v = out_edges[out_j];
//             if(v<i){
//                 if(v == out_v){
//                     if(reverse_out[out_j] == m){
//                         if(edge_type[j] == 2){
//                             cycle_num = 4;
//                             flow_num = 12;
//                         }else{
//                             cycle_num = 0;
//                             flow_num = 4;
//                         }
                        
//                         float c_value = (num_out_c[out_j] + num_out_c[reverse_out[out_j]] + cycle_num) / (2*(float)pow((long)((long)(degree[i]+1) * (long)(degree[v]+1)),0.5));
//                         float f_value = (num_out_f[out_j] + num_out_f[reverse_out[out_j]] + flow_num) / (6*(float)pow((long)((long)(degree[i]+1) * (long)(degree[v]+1)),0.5));
                        
//                         neighbor_order_index[j].vertex = v;
//                         neighbor_order_index[reverse[j]].vertex = i;

//                         neighbor_order_index[j].c = neighbor_order_index[reverse[j]].c = (int)(c_value*k);
//                         neighbor_order_index[j].f = neighbor_order_index[reverse[j]].f = (int)(f_value*k);
                        
//                     }
//                     j++;
//                     out_j++;
//                 }else{
//                     j++;
//                 }
//                 continue;
//             }
//             if(v == out_v){
//                 if(edge_type[j] == 2){
//                     cycle_num = 4;
//                     flow_num = 12;
//                 }else{
//                     cycle_num = 0;
//                     flow_num = 4;
//                 }
                
//                 float c_value = (num_out_c[out_j]+ num_out_c[reverse_out[out_j]]+ cycle_num) / (2*(float)pow((long)((long)(degree[i]+1) * (long)(degree[v]+1)),0.5));
//                 float f_value = (num_out_f[out_j]+ num_out_f[reverse_out[out_j]]+ flow_num) / (6*(float)pow((long)((long)(degree[i]+1) * (long)(degree[v]+1)),0.5));
                
//                 neighbor_order_index[j].vertex = v;
//                 neighbor_order_index[reverse[j]].vertex = i;

//                 neighbor_order_index[j].c = neighbor_order_index[reverse[j]].c = (int)(c_value*k);
//                 neighbor_order_index[j].f = neighbor_order_index[reverse[j]].f = (int)(f_value*k);
                
//                 j++;
//                 out_j++;
//             }else{
//                 j++;
//             }
//         }
//     }
    
//     for(int i = 0;i<n;i++){
//         sort(neighbor_order_index+pstart[i], neighbor_order_index+pstart[i+1],comp_nei_order_index);
//     }

//     int n_i = 0, now_f = k+1;
//     for(int i = 0;i<n;i++){
//         n_i = 0;
//         now_f = k+1;
//         for(uint32_t j = pstart[i];j<pstart[i+1];j++){
//             if(neighbor_order_index[j].f != now_f){
//                 neighbor_order_index_num[i][n_i++] = j;
//                 now_f = neighbor_order_index[j].f;
//             }
//         }
//         neighbor_order_index_num[i][n_i++] = pstart[i+1];
//         neighbor_num_pos[i] = n_i;
//     }
// }

bool comp5(int s1, int s2){
    return s1>s2;
}

//void Graph::getCoreOrder4(){
//    if(hash_c_pos == NULL) hash_c_pos = new int[n];
//    core_order_index_num.resize(max_degree+3, vector<uint32_t>(k+1));
//    if(core_num_pos == NULL) core_num_pos = new uint32_t[max_degree+3];
//    if(core_order_index == NULL) core_order_index = new k_index_core[(long)(1.5*edge_m)];
//    uint32_t core_order_indexi = 0;
//    int *ca_v = new int[max_degree];
//    int *ca_v2 = new int[max_degree];
//    int ca_vi = 0;
//    for(int i = 0;i<n;i++){
//        if(degree[i]<1) continue;
//        for(uint32_t j = pstart[i]; j<pstart[i+1];j++){//f-c
//            hash_c_pos[neighbor_order_c2[j].vertex] = (int)(neighbor_order_c2[j].value*k);
//        }
//
//        int min_k = (int)(neighbor_order_c2[pstart[i]].value*k);
//
//        int max_pos = -1;
//        int max_pos_f = k+1;
//        for(uint32_t j = pstart[i]; j<pstart[i+1];j++){
//            int v = neighbor_order_f2[j].vertex;
//            if(hash_c_pos[v] > max_pos){
//                max_pos = hash_c_pos[v];
//                //save
//                if((int)(neighbor_order_f2[j].value*k)<max_pos_f){
//                    max_pos_f = (int)(neighbor_order_f2[j].value*k);
//                    core_order_index[core_order_indexi].mu = 2;
//                    core_order_index[core_order_indexi].c = max_pos;
//                    core_order_index[core_order_indexi].f = max_pos_f;
//                    core_order_index[core_order_indexi++].vertex = i;
//                }else{
//                    core_order_index[core_order_indexi-1].c = max_pos;
//                }
//
//            }
//
//            if(max_pos == min_k) break;
//        }
//
//        if(degree[i]<2) continue;
//
//        core_order_index[core_order_indexi].mu = degree[i]+1;
//        core_order_index[core_order_indexi].c = (int)(neighbor_order_c2[pstart[i+1]-1].value*k);
//        core_order_index[core_order_indexi].f = (int)(neighbor_order_f2[pstart[i+1]-1].value*k);
//        core_order_index[core_order_indexi++].vertex = i;
//
//        int pos_up = k, pos_down = 0, pos_mid = 0, pos2;
//
//        ca_vi = 0;
//        ca_v2[ca_vi++] = hash_c_pos[neighbor_order_f2[pstart[i]].vertex];
//
//
//
//        for(int i2 = 1;i2<degree[i]-1;i2++){
//
//            ca_v2[ca_vi++] = hash_c_pos[neighbor_order_f2[pstart[i]+i2].vertex];
//
//            memcpy(ca_v,ca_v2,(i2+1)*sizeof(int));
////
//            if(ca_v[i2]>ca_v[i2-1]){
//                sort(ca_v, ca_v+i2+1, comp5);
//                memcpy(ca_v2,ca_v,(i2+1)*sizeof(int));
//            }
//
//            pos_down = ca_v[0];
//            pos_up = ca_v[i2];
//            pos_mid = ca_v[i2-1];
//
//            max_pos_f = (int)(neighbor_order_f2[pstart[i]+i2].value*k);
//
//            core_order_index[core_order_indexi].mu = i2+2;
//            core_order_index[core_order_indexi].c = pos_up;
//            core_order_index[core_order_indexi].f = max_pos_f;
//            core_order_index[core_order_indexi++].vertex = i;
//
//            for(uint32_t j2 = pstart[i]+i2+1;j2<pstart[i+1];j2++){
//                if(pos_down == min_k && pos_down - pos_up <= 1) break;
//                pos2 = hash_c_pos[neighbor_order_f2[j2].vertex];
//
//                if(pos2<=pos_up) continue;
//                else if(pos2>pos_down){
//
//                    if(pos_mid<pos_up){
//                        pos_up = pos_mid;
//                        if((int)(neighbor_order_f2[j2].value*k)<max_pos_f){
//                            max_pos_f = (int)(neighbor_order_f2[j2].value*k);
//                            core_order_index[core_order_indexi].mu = i2+2;
//                            core_order_index[core_order_indexi].c = pos_up;
//                            core_order_index[core_order_indexi].f = max_pos_f;
//                            core_order_index[core_order_indexi++].vertex = i;
//                        }else{
//                            core_order_index[core_order_indexi-1].c = pos_up;
//                        }
//
//                    }
//                    pos_down = pos2;
////                    ca_v[i2] = pos2;
//                    memcpy(ca_v+1,ca_v,(i2)*sizeof(int));
//                    ca_v[0] = pos2;
////                    sort(ca_v, ca_v+i2+1, comp5);
//                    pos_mid = ca_v[i2-1];
//                }else if (pos2 < pos_mid && pos2 > pos_up){
//                    //save
//                    pos_up = pos2;
//
//                    if((int)(neighbor_order_f2[j2].value*k)<max_pos_f){
//                        max_pos_f = (int)(neighbor_order_f2[j2].value*k);
//                        core_order_index[core_order_indexi].mu = i2+2;
//                        core_order_index[core_order_indexi].c = pos_up;
//                        core_order_index[core_order_indexi].f = max_pos_f;
//                        core_order_index[core_order_indexi++].vertex = i;
//                    }else{
//                        core_order_index[core_order_indexi-1].c = pos_up;
//                    }
//                }else if (pos2 < pos_down && pos2 > pos_mid){
//
//                    if(pos_mid<pos_up){
//                        pos_up = pos_mid;
//                        if((int)(neighbor_order_f2[j2].value*k)<max_pos_f){
//                            max_pos_f = (int)(neighbor_order_f2[j2].value*k);
//                            core_order_index[core_order_indexi].mu = i2+2;
//                            core_order_index[core_order_indexi].c = pos_up;
//                            core_order_index[core_order_indexi].f = max_pos_f;
//                            core_order_index[core_order_indexi++].vertex = i;
//                        }else{
//                            core_order_index[core_order_indexi-1].c = pos_up;
//                        }
//                    }
////                    int i3 = i2-2;
////                    while(ca_v[i3]<pos2){
////                        i3--;
////                    }
//                    int i3 = i2-2;
//                    int low = i2-2, high = 0, mid;
//
//                    while (low>high) {
//                        mid = (high+low)/2;
//                        if(pos2>ca_v[mid]) low = mid-1;
//                        else if(pos2<ca_v[mid+1]) high = mid+1;
//                        else{
//                            i3 = mid;
//                            break;
//                        }
//                    }
//
//                    if(low == high && pos2==ca_v[low]) i3 = low;
//
//
//                    memcpy(ca_v+i3+2,ca_v+i3+1,(i2-i3)*sizeof(int));
//                    ca_v[i3+1] = pos2;
////                    ca_v[i2] = pos2;
////                    sort(ca_v, ca_v+i2+1, comp5);
//
//                    pos_mid = ca_v[i2-1];
//                }
//            }
//        }
//    }
//
//    sort(core_order_index,core_order_index + core_order_indexi, comp_core_order_index);
//
//    int now_mu = 2, now_j = k+1, now_j_i = 0;
//    for(uint32_t j = 0;j<core_order_indexi;j++){
//        if(core_order_index[j].mu>now_mu){
//            core_order_index_num[now_mu][now_j_i++] = j;
//            core_num_pos[now_mu] = now_j_i;
//            now_j_i = 0;
//            now_j = k+1;
//            now_mu = core_order_index[j].mu;
//        }
//        if(core_order_index[j].f != now_j){
//            now_j = core_order_index[j].f;
//            core_order_index_num[now_mu][now_j_i++] = j;
//        }
////        core_order_index_num2[core_order_index[j].c][core_order_index[j].f]++;
//    }
//    core_order_index_num[max_degree+1][now_j_i++] = core_order_indexi;
//    core_num_pos[max_degree+1] = now_j_i;
//
//    cout<<edge_m<<" "<<core_order_indexi<<" "<<(float)(core_order_indexi - edge_m)/(float)edge_m<<endl;
//
//    if(ca_v!=NULL){
//        delete [] ca_v; ca_v = NULL;
//    }
//    if(ca_v2!=NULL){
//        delete [] ca_v2; ca_v2 = NULL;
//    }
//
//}

void Graph::getCoreOrder4_k(){
    // if(hash_c_pos == NULL) hash_c_pos = new int[n];
    core_order_index_num.resize(max_degree+5, vector<uint32_t>(k+3));
    if(core_num_pos == NULL) core_num_pos = new uint32_t[max_degree+5];
    for(int idx = 0; idx < max_degree + 5; ++idx) {
        core_num_pos[idx] = 0;
    }
    if(core_order_index != NULL) {
        delete [] core_order_index;
        core_order_index = NULL;
    }

    vector<k_index_core> core_entries;
    core_entries.reserve((size_t)edge_m);

    const int dim = k + 2;
    vector<int> matrix_f_c((size_t)dim * (size_t)dim, 0);
    vector<int> matrix_f_c_min((size_t)dim * (size_t)dim, 0);
    auto midx = [dim](int x, int y) -> size_t {
        return (size_t)x * (size_t)dim + (size_t)y;
    };
    //cout<<111<<endl;
    // int matrix_f_c_test[k+2][k+2];
    
//    int matrix_f_c_test[k+2][k+2];
//    for(int matrix_i = 0; matrix_i < k+2;matrix_i++){
//        for(int matrix_j = 0; matrix_j < k+2;matrix_j++){
//            matrix_f_c_test[matrix_i][matrix_j] = 0;
//        }
//    }
    
    if(order_c_ == NULL) order_c_ = new int[max_degree+2];
    int order_c_i = 0;
    
//    int over_k_k = 0;
    for(int i = 0;i<n;i++){
        
//        if(i%10000 == 0) cout<<i<<endl;
    //    cout<<i<<endl;
//
//        if(i == 241990){
//            cout<<111<<endl;
//        }
        
        if(degree[i]<1) continue;
        
        order_c_i = 0;
        
        vector<int> max_len_x(dim, 0), max_len_y(dim, 0), max_len_x_op(dim, 0), max_len_y_op(dim, 0);
        int max_len_x_i = 0, max_len_y_i = 0, max_len_x_op_i = 0, max_len_y_op_i = 0;

        for(int64_t j = (int64_t)pstart[i+1] - 1; j > (int64_t)pstart[i]; --j){//f-c
            if(neighbor_order_index[j].f != neighbor_order_index[j-1].f){
//                if(i == 241990)
//                    cout<<max_len_x_i<<":"<<neighbor_order_index[j].f<<endl;
                max_len_x[neighbor_order_index[j].f] = max_len_x_i++;
                max_len_x_op[max_len_x_op_i++] = neighbor_order_index[j].f;
            }
        }
        max_len_x[neighbor_order_index[pstart[i]].f] = max_len_x_i++;
        max_len_x_op[max_len_x_op_i++] = neighbor_order_index[pstart[i]].f;
        
        
        
        for(uint32_t j = pstart[i]; j<pstart[i+1];j++){//f-c
            order_c_[order_c_i++] = neighbor_order_index[j].c;
        }
        
        sort(order_c_, order_c_ + order_c_i, comp5);
        
        if(order_c_i <= 0) continue;

        for(int64_t j = (int64_t)order_c_i - 1; j > 0; --j){//f-c
            if(order_c_[j] != order_c_[j-1]){
                
//                if(i == 241990)
//                    cout<<max_len_y_i<<":"<<order_c_[j]<<endl;
                max_len_y_op[max_len_y_op_i++] = order_c_[j];
                max_len_y[order_c_[j]] = max_len_y_i++;
            }
        }
        max_len_y[order_c_[0]] = max_len_y_i++;
        max_len_y_op[max_len_y_op_i++] = order_c_[0];
        
        for(int matrix_i = 0; matrix_i < max_len_x_i+1;matrix_i++){
            for(int matrix_j = 0; matrix_j < max_len_y_i+1;matrix_j++){
                matrix_f_c[midx(matrix_i, matrix_j)] = 0;
                matrix_f_c_min[midx(matrix_i, matrix_j)] = 0;
                // matrix_f_c_test[matrix_i][matrix_j] = 0;
            }
        }
        
        for(uint32_t j = pstart[i]; j<pstart[i+1];j++){//f-c
            matrix_f_c[midx(max_len_x[neighbor_order_index[j].f], max_len_y[neighbor_order_index[j].c])]++;
        //    matrix_f_c_test[neighbor_order_index[j].c][neighbor_order_index[j].f]++;
        }
        for(int matrix_i = max_len_x_i-1; matrix_i >= 0;matrix_i--){
            for(int matrix_j = max_len_y_i-2; matrix_j >= 0;matrix_j--){
                matrix_f_c[midx(matrix_i, matrix_j)] = matrix_f_c[midx(matrix_i, matrix_j)] + matrix_f_c[midx(matrix_i, matrix_j + 1)];
            }
        }
        for(int matrix_j = max_len_y_i-1; matrix_j >= 0;matrix_j--){
            for(int matrix_i = max_len_x_i-2; matrix_i >= 0;matrix_i--){
                matrix_f_c[midx(matrix_i, matrix_j)] = matrix_f_c[midx(matrix_i, matrix_j)] + matrix_f_c[midx(matrix_i + 1, matrix_j)];
            }
        }
        
        
        for(int matrix_i = max_len_x_i-1;matrix_i>=0;matrix_i--){
            for(int matrix_j = max_len_y_i-1;matrix_j>=0;matrix_j--){
                matrix_f_c_min[midx(matrix_i, matrix_j)] = max(matrix_f_c[midx(matrix_i + 1, matrix_j)], matrix_f_c[midx(matrix_i, matrix_j + 1)]);
            }
        }
        
//        for(int matrix_i = 0; matrix_i < max_len_x_i;matrix_i++){
//            for(int matrix_j = 0; matrix_j < max_len_y_i;matrix_j++){
//                cout<<matrix_f_c[matrix_i][matrix_j]<<" ";
//            }
//            cout<<endl;
//        }
        
        
        for(int matrix_i = max_len_x_i-1;matrix_i>=0;matrix_i--){
            for(int matrix_j = max_len_y_i-1;matrix_j>=0;matrix_j--){
                for(int matrix_min_i = matrix_f_c_min[midx(matrix_i, matrix_j)] + 1; matrix_min_i <= matrix_f_c[midx(matrix_i, matrix_j)]; matrix_min_i++){
                    k_index_core entry;
                    entry.mu = matrix_min_i + 1;
                    entry.c = max_len_y_op[matrix_j];
                    entry.f = max_len_x_op[matrix_i];
                    entry.vertex = i;
                    core_entries.push_back(entry);
                }
            }
        }
    }

    if(core_entries.empty()) return;

    sort(core_entries.begin(), core_entries.end(), comp_core_order_index);

    const uint32_t core_order_indexi = (uint32_t)core_entries.size();
    core_order_index = new k_index_core[core_order_indexi];
    for(uint32_t idx = 0; idx < core_order_indexi; ++idx) {
        core_order_index[idx] = core_entries[idx];
    }

    // int mu_test = core_order_index[0].mu;
    // int j_test = core_order_index[0].f;

//     ofstream file;
//    file.open("test.txt", ios::out);

//     for(uint32_t j = 0;j<core_order_indexi;j++){
//         if(core_order_index[j].mu!=mu_test){
//             file<<"test:"<<mu_test<<" "<<j_test<<" "<<j<<endl;
//             file<<"............................................................................................................................................"<<endl;
//             mu_test = core_order_index[j].mu;
//             j_test = core_order_index[j].f;
//         }else{
//             if(core_order_index[j].f!=j_test){
//                  file<<"test:"<<mu_test<<" "<<j_test<<" "<<j<<endl;
//                  file<<".........................................................................."<<endl;
//                  j_test = core_order_index[j].f;
//             }
//         }

//     }
    
    uint32_t now_mu = core_order_index[0].mu, now_j = core_order_index[0].f, now_j_i = 0;
    // core_num_pos[now_mu] = 0;
    core_order_index_num[now_mu][now_j_i++] = 0;
    for(uint32_t j = 0;j<core_order_indexi;j++){
//        cout<<core_order_index[j].mu<<" "<<core_order_index[j].f<<" "<<core_order_index[j].c<<endl;
        if(core_order_index[j].mu == now_mu){
            if(core_order_index[j].f != now_j){
                now_j = core_order_index[j].f;
                core_order_index_num[now_mu][now_j_i++] = j;
            }
        }else{
            core_order_index_num[now_mu][now_j_i++] = j;
            core_num_pos[now_mu] = now_j_i;
            now_j_i = 0;
            now_mu = core_order_index[j].mu;
            now_j = core_order_index[j].f;
            core_order_index_num[now_mu][now_j_i++] = j;
        }
    }
    core_order_index_num[now_mu][now_j_i++] = core_order_indexi;
    core_num_pos[now_mu] = now_j_i;

    // for(int i = 0;i<core_num_pos[2];i++){
    //     cout<<"f:"<<core_order_index_num[2][i]<<" "<<core_order_index[core_order_index_num[2][i]].f<<endl;
    // }
    
    
//    cout<<"over_k_k:"<<over_k_k<<" n:"<<n<<" "<<(float)over_k_k/(float)n<<endl;

//    for(int matrix_i = 0; matrix_i < k+1;matrix_i++){
//        for(int matrix_j = 0; matrix_j < k+1;matrix_j++){
//            cout<<matrix_f_c_test[matrix_i][matrix_j]<<" ";
//        }
//        cout<<endl;
//    }
    // core_bm_ = new BitMap(n);
    // core_bm_judge_core = new BitMap(n);
}

int Graph::BinarySearch4_(unsigned int *a,unsigned int value,unsigned int b, unsigned int e){
    long low, high, mid;
    low = b;
    high = e-1;
    while(low<=high)
    {
        mid = (low+high)/2;
        if(a[mid]==value)
            return 1;
        if(a[mid]>value)
            high = mid-1;
        if(a[mid]<value)
            low = mid+1;
    }
    return 0;
}

void Graph::query_new_index(float parameter_c,float parameter_f, int parameterNum){//新的index方法
    if(parameterNum-1>max_degree) {
        cout<<"parameterNum -> too large!!!";
        return;
    }

    // cout<<13.1<<endl;

    if(pa_ == NULL) pa_ = new int[n];
    // cout<<13.2<<endl;
    for(int i = 0; i < n; i++) pa_[i] = i;
    // cout<<13.3<<endl;
    if(core_bm_ == NULL){
        // cout<<13.33<<endl;
        // cout<<n<<endl;
        core_bm_ = new BitMap(n);
    } 
    // cout<<13.4<<endl;
    if(core_bm_judge_core == NULL) core_bm_judge_core = new BitMap(n);
//    if(hash_c_pos_cluster == NULL) hash_c_pos_cluster = new int[n];
    int cur;
    // cout<<13.5<<endl;
    //if(all_core == NULL) 
    int *all_core = new int[n];
    int all_corei = 0;
    // cout<<13.6<<endl;
    int core_c = (int)(parameter_c*k), core_f = (int)(parameter_f*k), core_order_index_c, core_order_index_v;
    
    // cout<<13.7<<endl;
    for(int i = 0;i<core_num_pos[parameterNum];i++){
        // cout<<"f:"<<core_order_index[core_order_index_num[parameterNum][i]].f<<endl;
        if(core_order_index[core_order_index_num[parameterNum][i]].f>core_f){
            for(uint32_t j = core_order_index_num[parameterNum][i]; j<core_order_index_num[parameterNum][i+1];j++){

                core_order_index_c = core_order_index[j].c;
                core_order_index_v = core_order_index[j].vertex;
                if(core_order_index_c > core_c){
                //    cout<<"all_corei:"<<all_corei<<endl;
                    all_core[all_corei++] = core_order_index_v;
                    core_bm_judge_core->set(core_order_index_v,1);
                }else break;
            }
        }else break;
    }
    // cout<<13.8<<endl;
//    cout<<all_corei<<" "<<n<<endl;

    int all_corei2 = 0;
    sort(all_core, all_core+all_corei);
    // cout<<13.9<<endl;
    for(int i = 0;i<all_corei-1;i++) if(all_core[i] == all_core[i+1]) all_corei2++;
    
    // cout<<13.11<<endl;
    for(int i = 0;i<all_corei;i++){
        int now_id = all_core[i];
        if(core_bm_->get(now_id)) continue;
        
        cur = now_id;
        queue<int> q;
        q.push(now_id);
        
        core_bm_->set(now_id,1);
        
        while(!q.empty()){
            int q_id = q.front();
            q.pop();
            pa_[q_id] = cur;
            
            
            for(int nei_i = 0;nei_i<neighbor_num_pos[q_id];nei_i++){
                if(neighbor_order_index[neighbor_order_index_num[q_id][nei_i]].f>core_f){
                    for(uint32_t j = neighbor_order_index_num[q_id][nei_i]; j< neighbor_order_index_num[q_id][nei_i+1];j++){
                        if(neighbor_order_index[j].c>core_c){
                            int nei_order_index_v = neighbor_order_index[j].vertex;
                            if(core_bm_->get(nei_order_index_v)) continue;

                            if(core_bm_judge_core->get(nei_order_index_v)){
                                core_bm_->set(nei_order_index_v,1);
                                q.push(nei_order_index_v);
                            }else{
                                result_non_core_.push_back(make_pair(cur,nei_order_index_v));
                            }
                        }else break;
                    }
                }else break;
            }
            
//            for(uint32_t j = pstart[q_id]; j<pstart[q_id+1];j++){//f-c
//                if(neighbor_order_c2[j].value < parameter_c) break;
//                hash_c_pos_cluster[neighbor_order_c2[j].vertex] = q_id;
//            }
//            for(uint32_t j = pstart[q_id]; j<pstart[q_id+1];j++){//f-c
//                if(neighbor_order_f2[j].value < parameter_f) break;
//                int nei_order_index_v = neighbor_order_f2[j].vertex;
//                if(hash_c_pos_cluster[nei_order_index_v] == q_id){
//                    if(core_bm_->get(nei_order_index_v)) continue;
//
//                    if(core_bm_judge_core->get(nei_order_index_v)){
//                        core_bm_->set(nei_order_index_v,1);
//                        q.push(nei_order_index_v);
//                    }else{
//                        result_non_core_.push_back(make_pair(cur,nei_order_index_v));
//                    }
//                }
//            }
        }
        ++cur;
    }
    if(all_core!=NULL){
        delete [] all_core; all_core = NULL;
    }
    
}
void Graph::query_na(float parameter_c,float parameter_f, int parameterNum){//旧的方法
    // return;
    if(parameterNum-1>max_degree) {
        cout<<"parameterNum -> too large!!!";
        return;
    }

    // cout<<17.11<<endl;

    if(pa_ == NULL) pa_ = new int[n+1];
    // cout<<17.12<<endl;
    for(int i = 0; i < n; i++) pa_[i] = i;
    // cout<<17.13<<endl;
    if(core_bm_ == NULL) core_bm_ = new BitMap(n);
    int cur;

//    if(hubs == NULL) hubs = new int[n];

    int all_corei = 0;
    // cout<<17.14<<endl;
    if(all_nei == NULL) all_nei = new int[max_degree+1];
    // cout<<17.15<<endl;
    int all_nei_num = 0;

    for(int i = 0;i<n;i++){
        // if(i%10000 == 0) 
        // if(i == 403) cout<<i<<endl;
        if(degree[i]<parameterNum - 1) continue;
        if(neighbor_order_c2[pstart[i] + parameterNum - 2].value < parameter_c || neighbor_order_f2[pstart[i] + parameterNum - 2].value < parameter_f) continue;
        if(core_bm_->get(i)) continue;
        all_nei_num = 0;
        getAllNei(parameter_c, parameter_f, i, all_nei,all_nei_num);
        if(all_nei_num < parameterNum - 1){
            continue;
        }
        
        cur = i;
        queue<int> q;
        all_corei++;
//        continue;

        pa_[i] = cur;
        // if(i == 403){
            
        //    cout<<i<<endl;
        // }
         

        core_bm_->set(i,1);

        // if(i == 403) cout<<i<<endl;

        for(int all_neii = 0;all_neii<all_nei_num;all_neii++){
            int nbr_id = all_nei[all_neii];
            if(core_bm_->get(nbr_id)) continue;

            if((pstart[nbr_id+1] - pstart[nbr_id] >= (parameterNum-1)) && neighbor_order_c2[pstart[nbr_id] + parameterNum - 2].value>=parameter_c && neighbor_order_f2[pstart[nbr_id] + parameterNum - 2].value>=parameter_f){
                core_bm_->set(nbr_id,1);
                all_corei++;
                q.push(nbr_id);
            }else{
                result_non_core_.push_back(make_pair(cur,nbr_id));
            }
        }
        // if(i == 403) cout<<i<<endl;
        while(!q.empty()){
            int q_id = q.front();
            q.pop();

            all_nei_num = 0;
            getAllNei(parameter_c, parameter_f, q_id, all_nei,all_nei_num);

            if(all_nei_num < parameterNum - 1){
                result_non_core_.push_back(make_pair(cur,q_id));
                core_bm_->set(q_id,0);
                all_corei--;
                continue;
            }
            pa_[q_id] = cur;
            for(int all_neii = 0;all_neii<all_nei_num;all_neii++){
                int nbr_id = all_nei[all_neii];
                if(core_bm_->get(nbr_id)) continue;

                if((pstart[nbr_id+1] - pstart[nbr_id] >= (parameterNum-1)) && neighbor_order_c2[pstart[nbr_id] + parameterNum - 2].value>=parameter_c && neighbor_order_f2[pstart[nbr_id] + parameterNum - 2].value>=parameter_f){
                    core_bm_->set(nbr_id,1);
                    q.push(nbr_id);
                    all_corei++;
                }else{
                    result_non_core_.push_back(make_pair(cur,nbr_id));
                }
            }
        }
        ++cur;
    }

    
}

int Graph::find_root(int u) {
    int x = u;
    while(pa_[x] != x) x = pa_[x];

    while(pa_[u] != x) {
        int tmp = pa_[u];
        pa_[u] = x;
        u = tmp;
    }

    return x;
}

void Graph::output(float parameter_c,float parameter_f, int parameterNum){
    unordered_set<int> res;
    if(all_c_num == NULL) all_c_num = new int[n];
    for(int i = 0; i < n; ++i){
        all_c_num[i] = 0;
        if(!core_bm_->get(i)) continue;
        if(res.find(find_root(i)) != res.end()) continue;

        res.insert(find_root(i));
    }
    // cout<<16.1<<endl;
    
    ofstream fout(str+string("/output/") + to_string(parameter_c) + string("-")+to_string(parameter_f)+ string("-")+to_string(parameterNum) + string("-") + to_string(k) + string(".txt"),ios::out); //创建待写入数据文件
//    fout<<res.size()<<" clusters found~"<<"\n";
    // cout<<16.2<<endl;
    for(int i = 0; i < n; ++i) {
        if(core_bm_->get(i)) {
//            all_hub_and_out[i] = 1;
            fout<<"c "<<i<<" "<<find_root(i)<<"\n";
            all_c_num[find_root(i)]++;
        }
    }
    // cout<<16.3<<endl;
    sort(result_non_core_.begin(), result_non_core_.end());
    result_non_core_.erase(unique(result_non_core_.begin(), result_non_core_.end()), result_non_core_.end());

    // cout<<16.4<<endl;

    for(unsigned int i = 0;i < result_non_core_.size();i ++) {
        int id = result_non_core_[i].second;
//        all_hub_and_out[id] = 1;
        int root = result_non_core_[i].first;
        if(core_bm_->get(id) && find_root(id) == root) continue;
        pa_[id] = root;
        fout<<"n "<<id<<" "<<root<<"\n";
        all_c_num[root]++;
    }

    // cout<<16.5<<endl;

    // for(int i = 0;i<hubs_num;i++){
    //     fout<<"h "<<hubs[i]<<"\n";
    // }
    // fout.close();
    
    // if(all_c_num!=NULL){
    //     delete [] all_c_num;
    //     all_c_num = NULL;
    // }
    // cout<<16.6<<endl;

    for(int i = 0;i<n;i++){
        if(core_bm_->get(i)){
            core_bm_->set(i, 0);
        }
        if(core_bm_judge_core->get(i)){
            core_bm_judge_core->set(i, 0);
        }
    }

    // cout<<16.7<<endl;
//    for(int i = 0; i < n; i++) pa_[i] = i;
    result_non_core_.clear();
    
    hubs_num = 0;
    
//    if(hash_c_pos_cluster!=NULL){
//        delete [] hash_c_pos_cluster; hash_c_pos_cluster = NULL;
//    }
    
}

void Graph::evaluateNeighborProxy(float parameter_c, float parameter_f,
                                  double &precision, double &recall,
                                  double &f1, double &rmse,
                                  uint64_t &tp, uint64_t &fp, uint64_t &fn){
    if(all_nei == NULL) all_nei = new int[max_degree + 1];

    const int core_c = (int)(parameter_c * k);
    const int core_f = (int)(parameter_f * k);

    tp = 0;
    fp = 0;
    fn = 0;
    long double sq_err = 0.0L;

    vector<int> exact;
    vector<int> pred;
    exact.reserve(max_degree + 1);
    pred.reserve(max_degree + 1);

    for(uint64_t v = 0; v < n; ++v){
        int all_nei_num = 0;
        getAllNei(parameter_c, parameter_f, (int)v, all_nei, all_nei_num);

        exact.assign(all_nei, all_nei + all_nei_num);

        pred.clear();
        for(int nei_i = 0; nei_i < (int)neighbor_num_pos[v]; ++nei_i){
            const uint32_t block_begin = neighbor_order_index_num[v][nei_i];
            if(neighbor_order_index[block_begin].f > core_f){
                const uint32_t block_end = neighbor_order_index_num[v][nei_i + 1];
                for(uint32_t j = block_begin; j < block_end; ++j){
                    if(neighbor_order_index[j].c > core_c){
                        pred.push_back(neighbor_order_index[j].vertex);
                    }else{
                        break;
                    }
                }
            }else{
                break;
            }
        }

        sort(pred.begin(), pred.end());
        pred.erase(unique(pred.begin(), pred.end()), pred.end());

        uint64_t inter = 0;
        size_t i = 0, j = 0;
        while(i < exact.size() && j < pred.size()){
            if(exact[i] == pred[j]){
                ++inter;
                ++i;
                ++j;
            }else if(exact[i] < pred[j]){
                ++i;
            }else{
                ++j;
            }
        }

        tp += inter;
        fp += (uint64_t)pred.size() - inter;
        fn += (uint64_t)exact.size() - inter;

        const long long diff = (long long)pred.size() - (long long)exact.size();
        sq_err += (long double)diff * (long double)diff;
    }

    precision = (tp + fp) ? (double)tp / (double)(tp + fp) : 1.0;
    recall = (tp + fn) ? (double)tp / (double)(tp + fn) : 1.0;
    f1 = (precision + recall > 0.0) ? (2.0 * precision * recall / (precision + recall)) : 0.0;
    rmse = (n > 0) ? sqrt((double)(sq_err / (long double)n)) : 0.0;
}

void Graph::evaluateCoreNeighborCombinedProxy(float parameter_c, float parameter_f, int parameterNum,
                                              double &core_precision, double &core_recall, double &core_f1, double &core_rmse,
                                              double &neighbor_precision, double &neighbor_recall,
                                              double &neighbor_f1, double &neighbor_rmse,
                                              double &combined_precision, double &combined_recall,
                                              double &combined_f1){
    if(all_nei == NULL) all_nei = new int[max_degree + 1];

    const int core_c = (int)(parameter_c * k);
    const int core_f = (int)(parameter_f * k);

    vector<char> exact_core(n, 0);
    vector<char> approx_core(n, 0);

    if(parameterNum - 1 <= max_degree){
        for(uint64_t v = 0; v < n; ++v){
            if(degree[v] < parameterNum - 1) continue;
            if(neighbor_order_c2[pstart[v] + parameterNum - 2].value < parameter_c) continue;
            if(neighbor_order_f2[pstart[v] + parameterNum - 2].value < parameter_f) continue;
            exact_core[v] = 1;
        }
    }

    if(core_num_pos != NULL && parameterNum >= 0 && parameterNum < (int)core_order_index_num.size()){
        const uint32_t block_pos = core_num_pos[parameterNum];
        for(uint32_t bi = 0; bi + 1 < block_pos; ++bi){
            const uint32_t begin = core_order_index_num[parameterNum][bi];
            if(core_order_index[begin].f <= core_f) break;
            const uint32_t end = core_order_index_num[parameterNum][bi + 1];
            for(uint32_t j = begin; j < end; ++j){
                if(core_order_index[j].c > core_c){
                    approx_core[core_order_index[j].vertex] = 1;
                }else{
                    break;
                }
            }
        }
    }

    uint64_t core_tp = 0, core_fp = 0, core_fn = 0;
    for(uint64_t v = 0; v < n; ++v){
        if(exact_core[v] && approx_core[v]) ++core_tp;
        else if(!exact_core[v] && approx_core[v]) ++core_fp;
        else if(exact_core[v] && !approx_core[v]) ++core_fn;
    }

    core_precision = (core_tp + core_fp) ? (double)core_tp / (double)(core_tp + core_fp) : 1.0;
    core_recall = (core_tp + core_fn) ? (double)core_tp / (double)(core_tp + core_fn) : 1.0;
    core_f1 = (core_precision + core_recall > 0.0)
                  ? (2.0 * core_precision * core_recall / (core_precision + core_recall))
                  : 0.0;
    core_rmse = (n > 0) ? sqrt((double)(core_fp + core_fn) / (double)n) : 0.0;

    uint64_t nei_tp = 0, nei_fp = 0, nei_fn = 0;
    long double sq_err = 0.0L;
    uint64_t eval_vertex_cnt = 0;

    vector<int> exact;
    vector<int> pred;
    exact.reserve(max_degree + 1);
    pred.reserve(max_degree + 1);

    for(uint64_t v = 0; v < n; ++v){
        if(!(exact_core[v] || approx_core[v])) continue;
        ++eval_vertex_cnt;

        int all_nei_num = 0;
        getAllNei(parameter_c, parameter_f, (int)v, all_nei, all_nei_num);
        exact.assign(all_nei, all_nei + all_nei_num);

        pred.clear();
        for(int nei_i = 0; nei_i < (int)neighbor_num_pos[v]; ++nei_i){
            const uint32_t block_begin = neighbor_order_index_num[v][nei_i];
            if(neighbor_order_index[block_begin].f > core_f){
                const uint32_t block_end = neighbor_order_index_num[v][nei_i + 1];
                for(uint32_t j = block_begin; j < block_end; ++j){
                    if(neighbor_order_index[j].c > core_c){
                        pred.push_back(neighbor_order_index[j].vertex);
                    }else{
                        break;
                    }
                }
            }else{
                break;
            }
        }

        sort(pred.begin(), pred.end());
        pred.erase(unique(pred.begin(), pred.end()), pred.end());

        uint64_t inter = 0;
        size_t i = 0, j = 0;
        while(i < exact.size() && j < pred.size()){
            if(exact[i] == pred[j]){
                ++inter;
                ++i;
                ++j;
            }else if(exact[i] < pred[j]){
                ++i;
            }else{
                ++j;
            }
        }

        nei_tp += inter;
        nei_fp += (uint64_t)pred.size() - inter;
        nei_fn += (uint64_t)exact.size() - inter;

        const long long diff = (long long)pred.size() - (long long)exact.size();
        sq_err += (long double)diff * (long double)diff;
    }

    neighbor_precision = (nei_tp + nei_fp) ? (double)nei_tp / (double)(nei_tp + nei_fp) : 1.0;
    neighbor_recall = (nei_tp + nei_fn) ? (double)nei_tp / (double)(nei_tp + nei_fn) : 1.0;
    neighbor_f1 = (neighbor_precision + neighbor_recall > 0.0)
                      ? (2.0 * neighbor_precision * neighbor_recall / (neighbor_precision + neighbor_recall))
                      : 0.0;
    neighbor_rmse = (eval_vertex_cnt > 0)
                        ? sqrt((double)(sq_err / (long double)eval_vertex_cnt))
                        : 0.0;

    const uint64_t total_tp = core_tp + nei_tp;
    const uint64_t total_fp = core_fp + nei_fp;
    const uint64_t total_fn = core_fn + nei_fn;

    combined_precision = (total_tp + total_fp) ? (double)total_tp / (double)(total_tp + total_fp) : 1.0;
    combined_recall = (total_tp + total_fn) ? (double)total_tp / (double)(total_tp + total_fn) : 1.0;
    combined_f1 = (combined_precision + combined_recall > 0.0)
                      ? (2.0 * combined_precision * combined_recall / (combined_precision + combined_recall))
                      : 0.0;
}

void Graph::getAllNei(float parameter_c, float parameter_f, int v, int *all_nei, int &all_nei_num){
    if(nei == NULL) nei = new int[2*max_degree];
    int neii = 0;
    for(int i = pstart[v];i<pstart[v+1];i++){
        if(neighbor_order_c2[i].value>=parameter_c){
            nei[neii] = neighbor_order_c2[i].vertex;
            neii++;
        }else break;
    }
    for(int i = pstart[v];i<pstart[v+1];i++){
        if(neighbor_order_f2[i].value>=parameter_f){
            nei[neii] = neighbor_order_f2[i].vertex;
            neii++;
        }else break;
    }
    sort(nei, nei + neii);

    for(int i = 0;i<neii-1;i++){
        if(nei[i] == nei[i+1]){
            all_nei[all_nei_num] = nei[i];
            i++;
            all_nei_num++;
        }
    }
}

