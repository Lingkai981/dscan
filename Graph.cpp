//
//  Graph.cpp
//  index11
//
//  Created by 孟令凯 on 2021/5/9.
//

#include "Graph.hpp"
#include <algorithm>
#include "math.h"

Graph::Graph(const char *_dir){
    this->str = string(_dir);
    n = m = edge_m = max_degree = hubs_num = 0;
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
    num_c= NULL;
    num_f= NULL;
    num_out_c= NULL;
    num_out_f= NULL;
    neighbor_order_c2= NULL;
    neighbor_order_f2= NULL;
    max_degree_num = NULL;
    edge_type = NULL;
    
    pa_ = NULL;
    core_bm_ = NULL;
    query_visited = NULL;
    hubs = NULL;
    all_nei = NULL;
    all_cluster_vertices = NULL;
    nei = NULL;
    all_v_in_cluster = NULL;
    all_cluster_start = NULL;
    core = NULL;
    all_core = NULL;
}
Graph::~Graph(){
    if(core!=NULL){
        delete [] core; core = NULL;
    }
    if(all_core!=NULL){
        delete [] all_core; all_core = NULL;
    }
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
    if(num_c!=NULL){
        delete [] num_c;
        num_c = NULL;
    }
    if(num_f!=NULL){
        delete [] num_f;
        num_f = NULL;
    }
    if(num_out_c!=NULL){
        delete [] num_out_c;
        num_out_c = NULL;
    }
    if(num_out_f!=NULL){
        delete [] num_out_f;
        num_out_f = NULL;
    }
    if(reverse_out_bool!=NULL){
        delete [] reverse_out_bool;
        reverse_out_bool = NULL;
    }
    if(query_visited!=NULL){
        delete [] query_visited;
        query_visited = NULL;
    }
    if(hubs!=NULL){
        delete [] hubs;
        hubs = NULL;
    }
   
    if(neighbor_order_f2!=NULL){
//        delete[] neighbor_order_f[0];
//        for (int i = 0; i < edge_m; i++) {
//            neighbor_order_f[i] = NULL;
//        }
        delete[] neighbor_order_f2;
        neighbor_order_f2 = NULL;
    }
    if(max_degree_num!=NULL){
        delete [] max_degree_num;
        max_degree_num = NULL;
    }
    
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
    if(all_cluster_vertices!=NULL){
        delete [] all_cluster_vertices; all_cluster_vertices = NULL;
    }
    if(all_v_in_cluster!=NULL){
        delete [] all_v_in_cluster; all_v_in_cluster = NULL;
    }
    if(all_cluster_start!=NULL){
        delete [] all_cluster_start; all_cluster_start = NULL;
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
    
    cout<<"read!"<<endl;
    getDegree();
    
    cout<<"getDegree!"<<endl;
    
    
}

int Graph::readGraph_update(){
    ifstream infile;   //输入流
    
    infile.open(str+"/degree.txt", ios::in);
    if (!infile.is_open()){
        cout<<"Open degree file failure"<<endl;
        exit(0);
    }
    infile>>n>>m;
    
    update_in_num.resize(n);
    update_out_num.resize(n);
    update_in_edges.resize(n);
    update_out_edges.resize(n);
    int starti = 0;
    while (!infile.eof())            // 若未到文件结束一直循环
    {
        infile >> update_out_num[starti] >> update_in_num[starti];
        starti++;
    }
    infile.close();
    
    infile.open(str+"/out_edges.txt", ios::in);
    if (!infile.is_open()){
        cout<<"Open out_edges file failure"<<endl;
        exit(0);
    }
    int point;
    int mm_;
    infile >> mm_;
    for(int i = 0;i<n;i++){
        for(int j = 0;j<update_out_num[i];j++){
            infile >> point;
            update_out_edges[i].push_back(point);
        }
    }
    infile.close();
    
    infile.open(str+"/in_edges.txt", ios::in);
    if (!infile.is_open()){
        cout<<"Open in_edges file failure"<<endl;
        exit(0);
    }
    infile >> mm_;
    for(int i = 0;i<n;i++){
        for(int j = 0;j<update_in_num[i];j++){
            infile >> point;
            update_in_edges[i].push_back(point);
        }
    }
    infile.close();
    
    cout<<"read!"<<endl;
    
    return n;
}

void Graph::creatIndex(){

    getTti();
    getNeighborOrder();
    writeIndex();
}


void Graph::getDegree(){
    if(pstart == NULL) pstart = new uint32_t[n+1];
    if(degree == NULL) degree = new int[n];
    pstart[0] = 0;
    if(edges == NULL)  edges = new unsigned int[(long)2*m];
    if(edge_type == NULL)  edge_type = new int[(long)2*m];
//    edges = new unsigned int[(long)2*m];
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

    cout<<edge_m<<endl;
    reverse = new uint32_t[(long)edge_m];
    reverse_out = new uint32_t[(long)(m+1)];
    reverse_out_bool = new bool[(long)(m+1)];
//    num_c = new unsigned int[(long)edge_m];
//    num_f = new unsigned int[(long)edge_m];
    num_out_c = new uint32_t[(long)(m+1)];
    num_out_f = new uint32_t[(long)(m+1)];
    for(long i = 0;i<m;i++) {
        num_out_c[i] = 0;
        num_out_f[i] = 0;
        reverse_out[i] = m;
        reverse_out_bool[i] = true;
    }
    num_out_c[(long)m] = 0;
    num_out_f[(long)m] = 0;
    reverse_out_bool[(long)m] = true;
    initReverse();
    initReverse_out();
}



int Graph::intersection_f(unsigned int *A, unsigned int *B,uint64_t a1, uint64_t a2, uint64_t b1, uint64_t b2){
    int outi = 0;
    uint64_t i = a1, j = b1;
    while(i<a2 && j<b2){
        if(A[i] == B[j]){
            //cout<<double_<<endl;
            num_out_f[i] = num_out_f[i];
            num_out_f[j] = num_out_f[j];
            outi++;
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

void Graph::getTti_na(){
    for(int i = 0; i < n; i++){
        for(uint64_t j = out_pstart[i]; j<out_pstart[i+1]; j++){
            unsigned int v = out_edges[j];
            int num = intersection_f(out_edges, out_edges, out_pstart[i],  out_pstart[i+1], out_pstart[v], out_pstart[v+1]);
            num_out_f[j] = num_out_f[j] + num;
            if(i < v){
                int num = intersection_c(in_edges, out_edges, in_pstart[i],  in_pstart[i+1], out_pstart[v], out_pstart[v+1], i);
                num_out_c[j] = num_out_c[j] + num;
            }
            
        }
    }
}

void Graph::getTti(){
    int *visit_out = new int[n];
    int *visit_in = new int[n];
    memset(visit_out, -1, sizeof(int)*n);
    memset(visit_in, -1, sizeof(int)*n);
    for(int i = 0; i < n; i++){
        if(out_pstart[i+1] - out_pstart[i] == 0) continue;
        for(unsigned int j = out_pstart[i]; j<out_pstart[i+1]; j++){
            visit_out[out_edges[j]] = j;
        }
        for(unsigned int j = in_pstart[i]; j<in_pstart[i+1]; j++){
            visit_in[in_edges[j]] = j;
        }
        for(unsigned int j = out_pstart[i]; j<out_pstart[i+1]; j++){
            unsigned int v = out_edges[j];
            if(((out_pstart[v+1] - out_pstart[v]) < (out_pstart[i+1] - out_pstart[i])) || (((out_pstart[v+1] - out_pstart[v]) == (out_pstart[i+1] - out_pstart[i])) && (i<v))){
                int double_ = 1;
                if(reverse_out[j] != m) double_++;
                for(unsigned int k = out_pstart[v]; k<out_pstart[v+1]; k++){

                    unsigned int w = out_edges[k];
                    if(w == i) continue;
//                    out_vertex judge = hash.find(w);
                    if(visit_out[w]>0){
                        //cout<<i<<" "<<v<<" "<<w<<" "<<double_<<endl;
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
            if(degree[i]/degree[v]>100||degree[v]/degree[i]>100) continue;
            if(visit_out[v]>0) continue;
            if(((out_pstart[v+1] - out_pstart[v]) < (out_pstart[i+1] - out_pstart[i])) || (((out_pstart[v+1] - out_pstart[v]) == (out_pstart[i+1] - out_pstart[i])) && (i<v))){
                for(unsigned int k = out_pstart[v]; k<out_pstart[v+1]; k++){
                    unsigned int w = out_edges[k];
                    if(w == i) continue;
//                    out_vertex judge = hash.find(w);
                    if(visit_out[w]>0){
                        //cout<<i<<" "<<v<<" "<<w<<endl;
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

void Graph::getNeighborOrder(){
    
    neighbor_order_c2 = new point[edge_m];
    neighbor_order_f2 = new point[edge_m];
    int cycle_num = 4, flow_num = 12;
    for(int i = 0;i<n;i++){
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
//                        cout<<num_out_c[out_j]<<" "<<num_out_f[out_j]<<endl;
                        neighbor_order_c2[reverse[j]].value = neighbor_order_c2[j].value = (num_out_c[out_j] + cycle_num) / (2*(float)pow((long)((long)(degree[i]+1) * (long)(degree[v]+1)),0.5));
                        neighbor_order_c2[j].vertex = v;
                        neighbor_order_c2[reverse[j]].vertex = i;
                        neighbor_order_c2[j].type = edge_type[j];
                        neighbor_order_c2[reverse[j]].type = edge_type[reverse[j]];
                        
                        neighbor_order_f2[reverse[j]].value = neighbor_order_f2[j].value = (num_out_f[out_j]+ flow_num) / (6*(float)pow((long)((long)(degree[i]+1) * (long)(degree[v]+1)),0.5));
                        neighbor_order_f2[j].vertex = v;
                        neighbor_order_f2[reverse[j]].vertex = i;
                        neighbor_order_f2[j].type = edge_type[j];
                        neighbor_order_f2[reverse[j]].type = edge_type[reverse[j]];
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
//                cout<<num_out_c[out_j]<<" "<<num_out_f[out_j]<<endl;
                neighbor_order_c2[reverse[j]].value = neighbor_order_c2[j].value = (num_out_c[out_j] + num_out_c[reverse_out[out_j]] + cycle_num) / (2*(float)pow((long)((long)(degree[i]+1) * (long)(degree[v]+1)),0.5));
                neighbor_order_c2[j].vertex = v;
                neighbor_order_c2[reverse[j]].vertex = i;
                neighbor_order_c2[j].type = edge_type[j];
                neighbor_order_c2[reverse[j]].type = edge_type[reverse[j]];
                
                neighbor_order_f2[reverse[j]].value = neighbor_order_f2[j].value = (num_out_f[out_j] + num_out_f[reverse_out[out_j]]+ flow_num) / (6*(float)pow((long)((long)(degree[i]+1) * (long)(degree[v]+1)),0.5));
                neighbor_order_f2[j].vertex = v;
                neighbor_order_f2[reverse[j]].vertex = i;
                neighbor_order_f2[j].type = edge_type[j];
                neighbor_order_f2[reverse[j]].type = edge_type[reverse[j]];
                
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
    }
}

void Graph::getNeighborOrder_na(){
    neighbor_order_c2 = new point[edge_m];
    neighbor_order_f2 = new point[edge_m];
    for(int i = 0;i<n;i++){
        if(i%10 == 0){
        for(int j = pstart[i]; j<pstart[i+1];j++){
            unsigned int v = edges[j];
            if(v<i) continue;
            
            int c_num = 0, f_num = 0;
            
            unsigned int l1 = pstart[i], r1 = pstart[i+1];
            unsigned int l2 = pstart[v], r2 = pstart[v+1];
            
            while (l1<r1 && l2<r2) {
                if(edges[l1] == edges[l2]){
                    int w = edges[l1];
                    vector<int> a = get_f_and_c_num(i,v,w);
                    c_num = c_num + a[0];
                    f_num = f_num + a[1];
                    l1++;
                    l2++;
                }else if(edges[l1] < edges[l2]) l1++;
                else l2++;
            }
            neighbor_order_c2[reverse[j]].value = neighbor_order_c2[j].value = (c_num + 4) / (2*(float)pow((long)((long)(degree[i]+1) * (long)(degree[v]+1)),0.5));
            
            neighbor_order_c2[j].vertex = v;
            neighbor_order_c2[reverse[j]].vertex = i;
            neighbor_order_c2[j].type = edge_type[j];
            neighbor_order_c2[reverse[j]].type = edge_type[reverse[j]];
            
            
            neighbor_order_f2[reverse[j]].value = neighbor_order_f2[j].value = (f_num + 12) / (6*(float)pow((long)((long)(degree[i]+1) * (long)(degree[v]+1)),0.5));
            
            neighbor_order_f2[j].vertex = v;
            neighbor_order_f2[reverse[j]].vertex = i;
            neighbor_order_f2[j].type = edge_type[j];
            neighbor_order_f2[reverse[j]].type = edge_type[reverse[j]];
        }
        sort(neighbor_order_c2+pstart[i], neighbor_order_c2+pstart[i+1],comp3);
        sort(neighbor_order_f2+pstart[i], neighbor_order_f2+pstart[i+1],comp3);
    }
    }
}
vector<int> Graph::get_f_and_c_num(int a,int b, int c){

    
    int a_to_b = BinarySearch4_(out_edges,b,out_pstart[a],out_pstart[a+1]);
    int b_to_a = BinarySearch4_(out_edges,a,out_pstart[b],out_pstart[b+1]);
    int a_to_c = BinarySearch4_(out_edges,c,out_pstart[a],out_pstart[a+1]);
    int c_to_a = BinarySearch4_(out_edges,a,out_pstart[c],out_pstart[c+1]);
    int c_to_b = BinarySearch4_(out_edges,b,out_pstart[c],out_pstart[c+1]);
    int b_to_c = BinarySearch4_(out_edges,c,out_pstart[b],out_pstart[b+1]);
    
    int all =a_to_b+b_to_a+a_to_c+c_to_a+c_to_b+b_to_c;
    
    int cNum = 0;
    int fNum = 0;
    
    if(all == 6){
        cNum = cNum + 2;
        fNum = fNum + 6;    }
    else if(all == 5){
        cNum = cNum + 1;
        fNum = fNum + 3;
    }else{
        if(a_to_b && b_to_c && c_to_a) cNum++;
        if(a_to_c && c_to_b && b_to_a) cNum++;
        if(a_to_b && c_to_b && c_to_a) fNum++;
        if(a_to_b && a_to_c && c_to_b) fNum++;
        if(a_to_b && b_to_c && a_to_c) fNum++;
        if(a_to_c && b_to_c && b_to_a) fNum++;
        if(b_to_a && b_to_c && c_to_a) fNum++;
        if(b_to_a && c_to_b && c_to_a) fNum++;
    }
    vector<int> cn;
    cn.push_back(cNum);
    cn.push_back(fNum);
    return cn;
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
void Graph::writeIndex(){
    ofstream fout(str+"/index/degree.txt",ios::out); //创建待写入数据文件
    fout<<n<<" "<<m<<" "<<edge_m<<" "<<max_degree<<"\n";
    for(int i = 0; i< n-1;i++) fout<<(long)degree[i]<<"\n";
    fout<<(long)degree[n-1];
    fout.close();

    ofstream fout2(str+"/index/nei_order_c.txt",ios::out); //创建待写入数据文件
    for(int i = 0; i< n-1;i++){
        for(uint64_t j = pstart[i]; j<pstart[i+1]; j++){
            fout2<<(long)neighbor_order_c2[j].vertex<<" "<<neighbor_order_c2[j].value<<"\n";
        }
    }
    for(uint64_t j = pstart[n-1]; j<pstart[n]-1; j++){
        fout2<<(long)neighbor_order_c2[j].vertex<<" "<<neighbor_order_c2[j].value<<"\n";
    }
    fout2<<(long)neighbor_order_c2[pstart[n]-1].vertex<<" "<<neighbor_order_c2[pstart[n]-1].value;
    fout2.close();

    ofstream fout3(str+"/index/nei_order_f.txt",ios::out); //创建待写入数据文件
    for(int i = 0; i< n-1;i++){
        for(uint64_t j = pstart[i]; j<pstart[i+1]; j++){
            fout3<<(long)neighbor_order_f2[j].vertex<<" "<<neighbor_order_f2[j].value<<"\n";
        }
    }
    for(uint64_t j = pstart[n-1]; j<pstart[n]-1; j++){
        fout3<<(long)neighbor_order_f2[j].vertex<<" "<<neighbor_order_f2[j].value<<"\n";
    }
    fout3<<(long)neighbor_order_f2[pstart[n]-1].vertex<<" "<<neighbor_order_f2[pstart[n]-1].value;
    fout3.close();
}

void Graph::readIndex(){
    ifstream infile;   //输入流
    uint32_t i = 0;
    
    infile.open(str+"/index/degree.txt", ios::in);
    if (!infile.is_open()){
        cout<<"Open degree file failure"<<endl;
        exit(0);
    }

    infile >> n >> m >> edge_m >> max_degree;
    cout << n<<" "<< m <<" "<< edge_m <<" "<< max_degree<<endl;

    if(pstart == NULL) pstart = new uint32_t[n+1];
    if(degree == NULL) degree = new int[n];

    pstart[0] = 0;

    
    while (!infile.eof())            // 若未到文件结束一直循环
    {
        infile >> degree[i];
        pstart[i+1] = degree[i] + pstart[i];
        i++;
    }
    infile.close();
    neighbor_order_c2 = new point[edge_m];
    
    neighbor_order_f2 = new point[edge_m];
    
    infile.open(str+"/index/nei_order_c.txt", ios::in);
    if (!infile.is_open()){
        cout<<"Open nei_order_c file failure"<<endl;
        exit(0);
    }
    i=0;
    while (!infile.eof())            // 若未到文件结束一直循环
    {
        infile >> neighbor_order_c2[i].vertex >> neighbor_order_c2[i].value;
        i++;
    }
    infile.close();
    cout<<1<<endl;
    infile.open(str+"/index/nei_order_f.txt", ios::in);
    if (!infile.is_open()){
        cout<<"Open nei_order_f file failure"<<endl;
        exit(0);
    }
    i=0;
    while (!infile.eof())            // 若未到文件结束一直循环
    {
        infile >> neighbor_order_f2[i].vertex >> neighbor_order_f2[i].value;
        i++;
    }
    infile.close();
}



void Graph::query(float parameter1,float parameter2, int parameterNum){
    if(parameterNum-1>max_degree) {
        cout<<"parameterNum -> too large!!!";
        return;
    }
    
    int core_num_real = 0;
    int core_num_real_no = 0;

    if(pa_ == NULL) pa_ = new int[n];
    for(int i = 0; i < n; i++) pa_[i] = i;
    if(core_bm_ == NULL) core_bm_ = new BitMap(n);
    int cur;

    if(hubs == NULL) hubs = new int[n];


    if(all_nei == NULL) all_nei = new int[max_degree+1];
    int all_nei_num = 0;
    hubs_num = 0;
    if(all_cluster_vertices == NULL) all_cluster_vertices = new int[n];
    int all_cluster_verticesi = 0;
    if(all_cluster_start == NULL) all_cluster_start = new int[n];
    int all_cluster_starti = 1;
    all_cluster_start[0] = 0;
    if(query_visited == NULL) query_visited = new bool[n];
    memset(query_visited,false,sizeof(bool)*n);

    for(int i = 0;i<n;i++){


        if(degree[i]<parameterNum - 1) continue;

        if(neighbor_order_c2[pstart[i] + parameterNum - 2].value < parameter1 || neighbor_order_f2[pstart[i] + parameterNum - 2].value < parameter2) continue;

        if(core_bm_->get(i)) continue;
//        core_num_real++;
        all_nei_num = 0;
        getAllNei(parameter1, parameter2, i, all_nei,all_nei_num);
        if(all_nei_num < parameterNum - 1){
//            core_num_real_no++;
            continue;
        }

        cur = i;

//        cout<<i<<endl;

        queue<int> q;

        pa_[i] = cur;
        core_bm_->set(i,1);

        all_cluster_vertices[all_cluster_verticesi++] = i;
        query_visited[i] = true;

        for(int all_neii = 0;all_neii<all_nei_num;all_neii++){
            int nbr_id = all_nei[all_neii];
            if(core_bm_->get(nbr_id)) continue;

            if((pstart[nbr_id+1] - pstart[nbr_id] >= (parameterNum-1)) && neighbor_order_c2[pstart[nbr_id] + parameterNum - 2].value>=parameter1 && neighbor_order_f2[pstart[nbr_id] + parameterNum - 2].value>=parameter2){
                core_bm_->set(nbr_id,1);
                q.push(nbr_id);
            }else{
                result_non_core_.push_back(make_pair(cur,nbr_id));
                all_cluster_vertices[all_cluster_verticesi++] = nbr_id;
                query_visited[nbr_id] = true;
            }
        }
        while(!q.empty()){
            int q_id = q.front();
            q.pop();

            all_nei_num = 0;
            getAllNei(parameter1, parameter2, q_id, all_nei,all_nei_num);

            if(all_nei_num < parameterNum - 1){
                result_non_core_.push_back(make_pair(cur,q_id));
                all_cluster_vertices[all_cluster_verticesi++] = q_id;
                query_visited[q_id] = true;
                core_bm_->set(q_id,0);
                core_num_real_no++;
                continue;
            }
            pa_[q_id] = cur;
//            core_bm_->set(q_id,1);

            all_cluster_vertices[all_cluster_verticesi++] = q_id;
            query_visited[q_id] = true;

            for(int all_neii = 0;all_neii<all_nei_num;all_neii++){
                int nbr_id = all_nei[all_neii];
                if(core_bm_->get(nbr_id)) continue;

                if((pstart[nbr_id+1] - pstart[nbr_id] >= (parameterNum-1)) && neighbor_order_c2[pstart[nbr_id] + parameterNum - 2].value>=parameter1 && neighbor_order_f2[pstart[nbr_id] + parameterNum - 2].value>=parameter2){
                    core_bm_->set(nbr_id,1);
                    core_num_real++;
                    q.push(nbr_id);
                }else{
                    result_non_core_.push_back(make_pair(cur,nbr_id));
                    all_cluster_vertices[all_cluster_verticesi++] = nbr_id;
                    query_visited[nbr_id] = true;
                }
            }
        }
        all_cluster_start[all_cluster_starti++] = all_cluster_verticesi;
        ++cur;
    }
    HashSet hubs_candidate_set(n,2);
    vector<int> hubs_single;

    for(int i = 0; i<all_cluster_starti-1;i++){
        sort(all_cluster_vertices + all_cluster_start[i], all_cluster_vertices + all_cluster_start[i+1]);

        for(int j = all_cluster_start[i]; j<all_cluster_start[i+1]; j++){
            if(j>all_cluster_start[i] && all_cluster_vertices[j] == all_cluster_vertices[j-1]) continue;

            int cluster_v = all_cluster_vertices[j];
    //
            for(uint64_t k = pstart[cluster_v];k<pstart[cluster_v + 1];k++){
                if(query_visited[neighbor_order_c2[k].vertex]) continue;
                hubs_single.push_back(neighbor_order_c2[k].vertex);
            }
        }

        sort(hubs_single.begin(), hubs_single.end());
        for(int hubs_singlei = 0;hubs_singlei<hubs_single.size();hubs_singlei++){
            if(hubs_singlei>0 && hubs_single[hubs_singlei] == hubs_single[hubs_singlei-1]) continue;

            if(hubs_candidate_set.find_query(hubs_single[hubs_singlei])){
                hubs[hubs_num++] = hubs_single[hubs_singlei];
                query_visited[hubs_single[hubs_singlei]] = true;
            }else{
                hubs_candidate_set.insert_query(hubs_single[hubs_singlei]);
            }
        }
        hubs_single.clear();

    }
//    hubs_num = 0;
}

void Graph::query4(float parameter1,float parameter2, int parameterNum){
    if(parameterNum-1>max_degree) {
        cout<<"parameterNum -> too large!!!";
        return;
    }
    
//    int core_num_real = 0;
//    int core_num_real_no = 0;

    if(pa_ == NULL) pa_ = new int[n];
    for(int i = 0; i < n; i++) pa_[i] = i;
    if(core_bm_ == NULL) core_bm_ = new BitMap(n);
    int cur;

    if(hubs == NULL) hubs = new int[n];


    if(all_nei == NULL) all_nei = new int[max_degree+1];
    int all_nei_num = 0;
    hubs_num = 0;
    if(all_cluster_vertices == NULL) all_cluster_vertices = new int[n];
    int all_cluster_verticesi = 0;
    if(all_cluster_start == NULL) all_cluster_start = new int[n];
    int all_cluster_starti = 1;
    all_cluster_start[0] = 0;
    if(query_visited == NULL) query_visited = new bool[n];
    memset(query_visited,false,sizeof(bool)*n);

    for(int i = 0;i<n;i++){

        if(core_bm_->get(i)) continue;
//        core_num_real++;
        all_nei_num = 0;
        getAllNei(parameter1, parameter2, i, all_nei,all_nei_num);
        if(all_nei_num < parameterNum - 1){
//            core_num_real_no++;
            continue;
        }

        cur = i;

//        cout<<i<<endl;

        queue<int> q;
        q.push(i);

//        pa_[i] = cur;
        core_bm_->set(i,1);

        all_cluster_vertices[all_cluster_verticesi++] = i;
        query_visited[i] = true;


        while(!q.empty()){
            int q_id = q.front();
            q.pop();

            all_nei_num = 0;
            getAllNei(parameter1, parameter2, q_id, all_nei,all_nei_num);
            
            pa_[q_id] = cur;
//            core_bm_->set(q_id,1);

            all_cluster_vertices[all_cluster_verticesi++] = q_id;
            query_visited[q_id] = true;

            for(int all_neii = 0;all_neii<all_nei_num;all_neii++){
                int nbr_id = all_nei[all_neii];
                if(core_bm_->get(nbr_id)) continue;
                
                int all_nei_num2 = 0;
                getAllNei(parameter1, parameter2, nbr_id, all_nei,all_nei_num2);

                if(all_nei_num2 >= parameterNum - 1){
                    core_bm_->set(nbr_id,1);
//                    core_num_real++;
                    q.push(nbr_id);
                }else{
                    result_non_core_.push_back(make_pair(cur,nbr_id));
                    all_cluster_vertices[all_cluster_verticesi++] = nbr_id;
                    query_visited[nbr_id] = true;
                }
            }
        }
        all_cluster_start[all_cluster_starti++] = all_cluster_verticesi;
        ++cur;
    }
    
    HashSet hubs_candidate_set(n,2);
    vector<int> hubs_single;

    for(int i = 0; i<all_cluster_starti-1;i++){
        sort(all_cluster_vertices + all_cluster_start[i], all_cluster_vertices + all_cluster_start[i+1]);

        for(int j = all_cluster_start[i]; j<all_cluster_start[i+1]; j++){
            if(j>all_cluster_start[i] && all_cluster_vertices[j] == all_cluster_vertices[j-1]) continue;

            int cluster_v = all_cluster_vertices[j];
    //
            for(uint64_t k = pstart[cluster_v];k<pstart[cluster_v + 1];k++){
                if(query_visited[neighbor_order_c2[k].vertex]) continue;
                hubs_single.push_back(neighbor_order_c2[k].vertex);
            }
        }

        sort(hubs_single.begin(), hubs_single.end());
        for(int hubs_singlei = 0;hubs_singlei<hubs_single.size();hubs_singlei++){
            if(hubs_singlei>0 && hubs_single[hubs_singlei] == hubs_single[hubs_singlei-1]) continue;

            if(hubs_candidate_set.find_query(hubs_single[hubs_singlei])){
                hubs[hubs_num++] = hubs_single[hubs_singlei];
                query_visited[hubs_single[hubs_singlei]] = true;
            }else{
                hubs_candidate_set.insert_query(hubs_single[hubs_singlei]);
            }
        }
        hubs_single.clear();

    }
//    hubs_num = 0;

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

void Graph::output(float parameter1,float parameter2, int parameterNum){
    unordered_set<int> res;
    int *all_c_num = new int[n];
    for(int i = 0; i < n; ++i){
        all_c_num[i] = 0;
        if(!core_bm_->get(i)) continue;
        if(res.find(find_root(i)) != res.end()) continue;

        res.insert(find_root(i));
    }
    
    ofstream fout(str+"/output/" + to_string(parameter1) + "-"+to_string(parameter2)+ "-"+to_string(parameterNum) + ".txt",ios::out); //创建待写入数据文件
    fout<<res.size()<<" clusters found~"<<"\n";
    for(int i = 0; i < n; ++i) {
        if(core_bm_->get(i)) {
//            all_hub_and_out[i] = 1;
            fout<<"c "<<i<<" "<<find_root(i)<<"\n";
            all_c_num[find_root(i)]++;
        }
    }
    sort(result_non_core_.begin(), result_non_core_.end());
    result_non_core_.erase(unique(result_non_core_.begin(), result_non_core_.end()), result_non_core_.end());
    for(unsigned int i = 0;i < result_non_core_.size();i ++) {
        int id = result_non_core_[i].second;
//        all_hub_and_out[id] = 1;
        int root = result_non_core_[i].first;
        if(core_bm_->get(id) && find_root(id) == root) continue;
        pa_[id] = root;
        fout<<"n "<<id<<" "<<root<<"\n";
        all_c_num[root]++;
    }
    for(int i = 0;i<hubs_num;i++){
        fout<<"h "<<hubs[i]<<"\n";
    }
    fout.close();
    
    if(all_c_num!=NULL){
        delete [] all_c_num;
        all_c_num = NULL;
    }
    
    for(int i = 0;i<n;i++){
        if(core_bm_->get(i)){
            core_bm_->set(i, 0);
        }
    }
    for(int i = 0; i < n; i++) pa_[i] = i;
    result_non_core_.clear();
    
    hubs_num = 0;
    
}

void Graph::getAllNei(float parameter1, float parameter2, int v, int *all_nei, int &all_nei_num){
    if(nei == NULL) nei = new int[2*max_degree];
    int neii = 0;
    for(int i = pstart[v];i<pstart[v+1];i++){
        if(neighbor_order_c2[i].value>=parameter1){
            nei[neii] = neighbor_order_c2[i].vertex;
            neii++;
        }else break;
    }
    for(int i = pstart[v];i<pstart[v+1];i++){
        if(neighbor_order_f2[i].value>=parameter2){
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
int Graph::getAllNei3(float parameter1, float parameter2, int v){
    if(nei == NULL) nei = new int[2*max_degree];
    int neii = 0;
    for(int i = pstart[v];i<pstart[v+1];i++){
        if(neighbor_order_c2[i].value>=parameter1){
            nei[neii] = neighbor_order_c2[i].vertex;
            neii++;
        }else break;
    }
    for(int i = pstart[v];i<pstart[v+1];i++){
        if(neighbor_order_f2[i].value>=parameter2){
            nei[neii] = neighbor_order_f2[i].vertex;
            neii++;
        }else break;
    }
    sort(nei, nei + neii);
    int num = 0;
    for(int i = 0;i<neii-1;i++){
        if(nei[i] == nei[i+1]){
            i++;
            num++;
        }
    }
    return num;
}
void Graph::getAllNei2(float parameter1, float parameter2, int v, int *all_nei, int &all_nei_num){
    int *nei2 = new int[2*degree[v]];
    int neii = 0;
    for(int i = pstart[v];i<pstart[v+1];i++){
        if(neighbor_order_c2[i].value>=parameter1){
            nei2[neii] = neighbor_order_c2[i].vertex;
            neii++;
        }else break;
    }
    for(int i = pstart[v];i<pstart[v+1];i++){
        if(neighbor_order_f2[i].value>=parameter2){
            nei2[neii] = neighbor_order_f2[i].vertex;
            neii++;
        }else break;
    }
    sort(nei2, nei2 + neii);
    
    for(int i = 0;i<neii-1;i++){
        if(nei2[i] == nei2[i+1]){
            all_nei[all_nei_num] = nei2[i];
            i++;
            all_nei_num++;
        }
    }
    delete [] nei2; nei2 = NULL;
}


void Graph::readIndex_update(){
   
    
    ifstream infile;   //输入流
    
    infile.open(str+"/index/degree.txt", ios::in);
    if (!infile.is_open()){
        cout<<"Open degree file failure"<<endl;
        exit(0);
    }

    infile >> n >> m >> edge_m >> max_degree;
    cout << n<<" "<< m <<" "<< edge_m <<" "<< max_degree<<endl;
    
    nbr_mp_c.resize(n);
    nbr_mp_f.resize(n);
    update_degree.resize(n);
//    nbr_order_old_c.resize(n);
//    nbr_order_old_f.resize(n);
    
    int degreei = 0;
    while (!infile.eof())            // 若未到文件结束一直循环
    {
        infile >> update_degree[degreei];
        degreei++;
    }
    infile.close();
    
    
    int n_;
    float val_;
    infile.open(str+"/index/nei_order_c.txt", ios::in);
    if (!infile.is_open()){
        cout<<"Open nei_order_c file failure"<<endl;
        exit(0);
    }
    for(int i = 0;i<n;i++){
        for(int j = 0;j<update_degree[i];j++){
            infile >> n_ >> val_;
            nbr_mp_c[i].insert(make_pair(val_, n_));
//            nbr_order_old_c[i].push_back(val_);
        }
    }
    infile.close();
    
    infile.open(str+"/index/nei_order_f.txt", ios::in);
    if (!infile.is_open()){
        cout<<"Open nei_order_f file failure"<<endl;
        exit(0);
    }
    for(int i = 0;i<n;i++){
        for(int j = 0;j<update_degree[i];j++){
            infile >> n_ >> val_;
            nbr_mp_c[i].insert(make_pair(val_, n_));
//            nbr_order_old_f[i].push_back(val_);
        }
    }
    infile.close();
}

void Graph::insert(int u, int v){
    
    
    
    unordered_map<int,int> add_c;//获取新的cycle三角形结构
    unordered_map<int,int> add_f;//获取新的flow三角形结构
    int all_c = 0, all_f = 0;
    
    if(BinarySearch3(update_out_edges[u], v)){
        cout<<u<<"->"<<v<<" exist!!!"<<endl;
        return;
    }
    m++;//边增加
    get_add_c_and_f(add_c,add_f,u,v,all_c,all_f);
    update_in_num[v]++;
    update_out_num[u]++;
    update_in_edges[v].push_back(u);
    update_out_edges[u].push_back(v);
    sort(update_in_edges[v].begin(), update_in_edges[v].end());
    sort(update_out_edges[u].begin(), update_out_edges[u].end());
    
    if(BinarySearch3(update_out_edges[v], u)){
        edge_m--;
//            cout<<"case 3"<<endl;
        update_nei2(add_c,add_f,all_c,all_f,v,u);
        update_nei2(add_c,add_f,all_c,all_f,u,v);
    }else{
        edge_m++;//邻居数目增加
//            cout<<"case 4"<<endl;
        update_degree[u]++;
        update_degree[v]++;
        if(update_degree[v]>max_degree || update_degree[u]>max_degree){
            max_degree++;
        }
        update_nei1(add_c,add_f,all_c,all_f,v,u);
        update_nei1(add_c,add_f,all_c,all_f,u,v);
    }
    
  

    
}

void Graph::get_add_c_and_f(unordered_map<int,int> &add_c, unordered_map<int,int> &add_f, int u, int v, int &all_c, int &all_f){
    
    int ui = 0, vi = 0, ul = update_out_num[u], vl = update_out_num[v];
    
    while(ui<ul && vi < vl){
        
        if(update_out_edges[u][ui] == update_out_edges[v][vi]){
            add_f[update_out_edges[u][ui]]++;
            all_f++;
            ui++;
            vi++;
        }
        else if(update_out_edges[u][ui] < update_out_edges[v][vi]) ui++;
        else vi++;
    }
    
    ui = 0;
    vi = 0;
    vl = update_in_num[v];
    while(ui<ul && vi < vl){
        
        if(update_out_edges[u][ui] == update_in_edges[v][vi]){
            add_f[update_out_edges[u][ui]]++;
            all_f++;
            ui++;
            vi++;
        }
        else if(update_out_edges[u][ui] < update_in_edges[v][vi]) ui++;
        else vi++;
    }
    
    ui = 0;
    vi = 0;
    ul = update_in_num[u];
    while(ui<ul && vi < vl){
        
        if(update_in_edges[u][ui] == update_in_edges[v][vi]){
            add_f[update_in_edges[u][ui]]++;
            all_f++;
            ui++;
            vi++;
        }
        else if(update_in_edges[u][ui] < update_in_edges[v][vi]) ui++;
        else vi++;
    }
    
    ui = 0;
    vi = 0;
    vl = update_out_num[v];
    
    while(ui<ul && vi < vl){
        
        if(update_in_edges[u][ui] == update_out_edges[v][vi]){
            add_c[update_in_edges[u][ui]]++;
            all_c++;
            ui++;
            vi++;
        }
        else if(update_in_edges[u][ui] < update_out_edges[v][vi]) ui++;
        else vi++;
    }
}

void Graph::update_nei_order_c(int v, int w, float old_sim, float new_sim){
    auto pr = nbr_mp_c[v].equal_range(old_sim);
    if(pr.first != end(nbr_mp_c[v]))
    {
        for (auto iter = pr.first ; iter != pr.second; ++iter){
            if(iter->second == w){
                nbr_mp_c[v].erase(pr.first);
                nbr_mp_c[v].insert(make_pair(new_sim, iter->second));
                break;
            }
        }
    }
    
}
void Graph::update_nei_order_f(int v, int w, float old_sim, float new_sim){
    auto pr = nbr_mp_f[v].equal_range(old_sim);
    if(pr.first != end(nbr_mp_f[v]))
    {
        for (auto iter = pr.first ; iter != pr.second; ++iter){
            if(iter->second == w){
                nbr_mp_f[v].erase(pr.first);
                nbr_mp_f[v].insert(make_pair(new_sim, iter->second));
                break;
            }
        }
    }
}

void Graph::update_nei1(unordered_map<int,int> &add_c, unordered_map<int,int> &add_f, int all_c, int all_f, int u, int v){
    multimap<float, int, greater<float>> single = nbr_mp_c[u];
    nbr_mp_c[u].clear();
    for(multimap<float, int, greater<float>>::iterator it = single.begin();it!=single.end();++it){
        if(add_c.find(it->second)!=add_c.end()){
            int old_sim = round(it->first*2*(pow((update_degree[u]-1)*update_degree[it->second],0.5)));
            float new_sim = (float)(old_sim+add_c[it->second])/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
//            float old_ = it->first;
            update_nei_order_c(it->second, u, it->first, new_sim);
            nbr_mp_c[u].insert(make_pair(new_sim, it->second));
        }else{
            int old_sim = round(it->first*2*(pow((update_degree[u]-1)*update_degree[it->second],0.5)));
            float new_sim = (float)(old_sim)/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
//            float old_ = it->first;
            update_nei_order_c(it->second, u, it->first, new_sim);
            nbr_mp_c[u].insert(make_pair(new_sim, it->second));
        }
    }
    nbr_mp_c[u].insert(make_pair((float)(all_c)/(2*(pow(update_degree[u]*update_degree[v],0.5))), v));
    
    multimap<float, int, greater<float>> single2 = nbr_mp_f[u];
    nbr_mp_f[u].clear();
    for(multimap<float, int, greater<float>>::iterator it = single2.begin();it!=single2.end();++it){
        if(add_f.find(it->second)!=add_f.end()){
            int old_sim = round(it->first*2*(pow((update_degree[u]-1)*update_degree[it->second],0.5)));
            float new_sim = (float)(old_sim+add_f[it->second])/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
            update_nei_order_f(it->second, u, it->first, new_sim);
            nbr_mp_f[u].insert(make_pair(new_sim, it->second));
        }else{
            int old_sim = round(it->first*2*(pow((update_degree[u]-1)*update_degree[it->second],0.5)));
            float new_sim = (float)(old_sim)/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
            update_nei_order_f(it->second, u, it->first, new_sim);
            nbr_mp_f[u].insert(make_pair(new_sim, it->second));
        }
    }
    nbr_mp_f[u].insert(make_pair((float)(all_f+4)/(6*(pow(update_degree[u]*update_degree[v],0.5))), v));

}
void Graph::update_nei2(unordered_map<int,int> &add_c, unordered_map<int,int> &add_f, int all_c, int all_f, int u, int v){
    multimap<float, int, greater<float>> single = nbr_mp_c[u];
    nbr_mp_c[u].clear();
    for(multimap<float, int, greater<float>>::iterator it = single.begin();it!=single.end();++it){
        if(it->second == v){
            int old_sim = round(it->first*2*(pow((update_degree[u])*update_degree[it->second],0.5)));
            float new_sim = (float)(old_sim+all_c)/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
            nbr_mp_c[u].insert(make_pair(new_sim, it->second));
            continue;
        }
        if(add_c.find(it->second)!=add_c.end()){
            int old_sim = round(it->first*2*(pow((update_degree[u])*update_degree[it->second],0.5)));
            float new_sim = (float)(old_sim+add_c[it->second])/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
//            float old_ = it->first;
            update_nei_order_c(it->second, u, it->first, new_sim);
            nbr_mp_c[u].insert(make_pair(new_sim, it->second));
//            update_core_c(it->second, old_, new_sim);
        }else{
            nbr_mp_c[u].insert(make_pair(it->first, it->second));
        }
    }
    
    multimap<float, int, greater<float>> single2 = nbr_mp_f[u];
    nbr_mp_f[u].clear();
    for(multimap<float, int, greater<float>>::iterator it = single2.begin();it!=single2.end();++it){
        if(it->second == v){
            int old_sim = round(it->first*2*(pow((update_degree[u])*update_degree[it->second],0.5)));
            float new_sim = (float)(old_sim+all_f)/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
            nbr_mp_f[u].insert(make_pair(new_sim, it->second));
            continue;
        }
        if(add_f.find(it->second)!=add_f.end()){
            int old_sim = round(it->first*2*(pow((update_degree[u])*update_degree[it->second],0.5)));
            float new_sim = (float)(old_sim+add_f[it->second])/(6*(pow(update_degree[u]*update_degree[it->second],0.5)));

            update_nei_order_f(it->second, u, it->first, new_sim);
            nbr_mp_f[u].insert(make_pair(new_sim, it->second));
        }else{
            nbr_mp_f[u].insert(make_pair(it->first, it->second));
        }
    }
}

void Graph::remove(int u, int v){
    if(!BinarySearch3(update_out_edges[u], v)){
        cout<<u<<"->"<<v<<" not exist!!!"<<endl;
        return;
    }
    m--;
    unordered_map<int,int> add_c;
    unordered_map<int,int> add_f;
    int all_c = 0, all_f = 0;
    
    get_add_c_and_f(add_c,add_f,u,v,all_c,all_f);
    
    update_in_num[v]--;
    update_out_num[u]--;
    update_in_edges[v].erase(std::remove(update_in_edges[v].begin(),update_in_edges[v].end(),u),update_in_edges[v].end());
    update_out_edges[u].erase(std::remove(update_out_edges[u].begin(),update_out_edges[u].end(),v),update_out_edges[u].end());
    
    if(BinarySearch3(update_out_edges[v], u)){
        delete_update_nei2(add_c,add_f,all_c,all_f,u,v);
        delete_update_nei2(add_c,add_f,all_c,all_f,v,u);
    }else{
        edge_m--;
        update_degree[u]--;
        update_degree[v]--;
        
        delete_update_nei1(add_c,add_f,all_c,all_f,u,v);
        delete_update_nei1(add_c,add_f,all_c,all_f,v,u);
    }
}

void Graph::delete_update_nei1(unordered_map<int,int> &add_c, unordered_map<int,int> &add_f, int all_c, int all_f, int u, int v){//不存在v->u
    multimap<float, int, greater<float>> single = nbr_mp_c[u];
    nbr_mp_c[u].clear();
    for(multimap<float, int, greater<float>>::iterator it = single.begin();it!=single.end();++it){
        if(it->second == v){
            continue;
        }
        if(add_c.find(it->second)!=add_c.end()){
            int old_sim = round(it->first*2*(pow((update_degree[u]+1)*update_degree[it->second],0.5)));
            float new_sim = (float)(old_sim-add_c[it->second])/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
            float old_ = it->first;
            update_nei_order_c(it->second, u, old_, new_sim);
            nbr_mp_c[u].insert(make_pair(new_sim, it->second));
//            update_core_c(it->second, old_, new_sim);
        }else{
            int old_sim = round(it->first*2*(pow((update_degree[u]+1)*update_degree[it->second],0.5)));
            float new_sim = (float)(old_sim)/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
            float old_ = it->first;
            update_nei_order_c(it->second, u, old_, new_sim);
            nbr_mp_c[u].insert(make_pair(new_sim, it->second));
//            update_core_c(it->second, old_, new_sim);
        }
    }

    multimap<float, int, greater<float>> single2 = nbr_mp_f[u];
    nbr_mp_f[u].clear();
    for(multimap<float, int, greater<float>>::iterator it = single2.begin();it!=single2.end();++it){
        if(it->second == v){
            continue;
        }
        if(add_f.find(it->second)!=add_f.end()){
            int old_sim = round(it->first*6*(pow((update_degree[u]+1)*update_degree[it->second],0.5)));
            float new_sim = (float)(old_sim+add_f[it->second])/(6*(pow(update_degree[u]*update_degree[it->second],0.5)));
//            float old_ = it->first;
            update_nei_order_f(it->second, u, it->first, new_sim);
            nbr_mp_f[u].insert(make_pair(new_sim, it->second));
        }else{
            int old_sim = round(it->first*6*(pow((update_degree[u]+1)*update_degree[it->second],0.5)));
            float new_sim = (float)(old_sim)/(6*(pow(update_degree[u]*update_degree[it->second],0.5)));
            update_nei_order_f(it->second, u, it->first, new_sim);
            nbr_mp_f[u].insert(make_pair(new_sim, it->second));
        }
    }
}
void Graph::delete_update_nei2(unordered_map<int,int> &add_c, unordered_map<int,int> &add_f, int all_c, int all_f, int u, int v){
    multimap<float, int, greater<float>> single = nbr_mp_c[u];
    nbr_mp_c[u].clear();
    for(multimap<float, int, greater<float>>::iterator it = single.begin();it!=single.end();++it){
        if(it->second == v){
            int old_sim = round(it->first*2*(pow((update_degree[u])*update_degree[it->second],0.5)));
            float new_sim = (float)(old_sim-all_c)/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
            nbr_mp_c[u].insert(make_pair(new_sim, it->second));
            continue;
        }
        if(add_c.find(it->second)!=add_c.end()){
            int old_sim = round(it->first*2*(pow((update_degree[u])*update_degree[it->second],0.5)));
            float new_sim = (float)(old_sim-add_c[it->second])/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
//            float old_ = it->first;
            update_nei_order_c(it->second, u, it->first, new_sim);
            nbr_mp_c[u].insert(make_pair(new_sim, it->second));
//            update_core_c(it->second.vertex, old_, new_sim);
        }else{
            nbr_mp_c[u].insert(make_pair(it->first, it->second));
        }
    }
//    update_core_c(u, 0, 1);

    multimap<float, int, greater<float>> single2 = nbr_mp_f[u];
    nbr_mp_f[u].clear();
    for(multimap<float, int, greater<float>>::iterator it = single2.begin();it!=single2.end();++it){
        if(it->second == v){
            int old_sim = round(it->first*6*(pow((update_degree[u])*update_degree[it->second],0.5)));
            float new_sim = (float)(old_sim-all_f)/(6*(pow(update_degree[u]*update_degree[it->second],0.5)));
            nbr_mp_f[u].insert(make_pair(new_sim, it->second));
            continue;
        }
        if(add_f.find(it->second)!=add_f.end()){
            int old_sim = round(it->first*6*(pow((update_degree[u])*update_degree[it->second],0.5)));
            float new_sim = (float)(old_sim-add_f[it->second])/(6*(pow(update_degree[u]*update_degree[it->second],0.5)));
//            float old_ = it->first;
            update_nei_order_f(it->second, u, it->first, new_sim);
            nbr_mp_f[u].insert(make_pair(new_sim, it->second));
        }else{
            nbr_mp_f[u].insert(make_pair(it->first, it->second));
        }
    }
}

void Graph::update_write(){//写入文件
    ofstream degree_(str+"/index/degree.txt",ios::out); //创建待写入数据文件
    degree_<<(long)n<<" "<<(long)m<<" "<<(long)edge_m<<" "<<(long)max_degree;
    for(int i = 0; i< n;i++) degree_<<"\n"<<(long)update_degree[i];
    degree_.close();
    
    ofstream infile(str+"/degree.txt", ios::out);
    if (!infile.is_open()){
        cout<<"Open degree file failure"<<endl;
        exit(0);
    }
    infile<<n<<" "<<m;
    for(int i = 0; i<n;i++){
        infile<<"\n"<<(long)update_out_num[i]<<" "<<(long)update_in_num[i];
    }
    infile.close();
    
    ofstream write_in_edge(str+"/in_edges.txt", ios::out);
    write_in_edge<<m;
    for(int i = 0;i<n;i++){
        for(int j = 0;j<update_in_edges[i].size();j++){
            write_in_edge<<"\n"<<(long)update_in_edges[i][j];
        }
    }
    write_in_edge.close();
    
    ofstream write_out_edge(str+"/out_edges.txt", ios::out);
    write_out_edge<<m;
    for(int i = 0;i<n;i++){
        for(int j = 0;j<update_out_edges[i].size();j++){
            write_out_edge<<"\n"<<(long)update_out_edges[i][j];
        }
    }
    write_out_edge.close();
    
    
    ofstream fout2(str+"/index/nei_order_c.txt",ios::out); //创建待写入数据文件
    for(int i = 0; i< n-1;i++){
        for(multimap<float, int, greater<float>>::iterator it = nbr_mp_c[i].begin();it!=nbr_mp_c[i].end();++it){
            fout2<<(long)it->second<<" "<<it->first<<"\n";
        }
    }
    int nbr_mp_ci = update_degree[n-1];
    for(multimap<float, int, greater<float>>::iterator it = nbr_mp_c[n-1].begin();it!=nbr_mp_c[n-1].end();++it){
        if(nbr_mp_ci > 1)
            fout2<<(long)it->second<<" "<<it->first<<"\n";
        else
            fout2<<(long)it->second<<" "<<it->first;
        nbr_mp_ci--;
    }
    fout2.close();
    
    ofstream fout3(str+"/index/nei_order_f.txt",ios::out); //创建待写入数据文件
    for(int i = 0; i< n-1;i++){
        for(multimap<float, int, greater<float>>::iterator it = nbr_mp_f[i].begin();it!=nbr_mp_f[i].end();++it){
            fout2<<(long)it->second<<" "<<it->first<<"\n";
        }
    }
    int nbr_mp_fi = update_degree[n-1];
    for(multimap<float, int, greater<float>>::iterator it = nbr_mp_f[n-1].begin();it!=nbr_mp_f[n-1].end();++it){
        if(nbr_mp_fi > 1)
            fout2<<(long)it->second<<" "<<it->first<<"\n";
        else
            fout2<<(long)it->second<<" "<<it->first;
        nbr_mp_fi--;
    }
    fout3.close();
    
}

void Graph::insert_na(int u, int v){

    if(BinarySearch3(update_out_edges[u], v)){
        cout<<u<<"->"<<v<<" exist!!!"<<endl;
        
    }
    m++;

    update_in_num[v]++;
    update_out_num[u]++;
    update_in_edges[v].push_back(u);
    update_out_edges[u].push_back(v);
    sort(update_in_edges[v].begin(), update_in_edges[v].end());
    sort(update_out_edges[u].begin(), update_out_edges[u].end());

    if(BinarySearch3(update_out_edges[v], u)){
        
        update_nei2_4(v,u);
        update_nei2_4(u,v);
    }else{
        edge_m++;
        update_degree[u]++;
        update_degree[v]++;
        if(update_degree[v]>max_degree || update_degree[u]>max_degree){
            max_degree++;
        }
        update_nei1_4(v,u);
        update_nei1_4(u,v);
    }
        

}

void Graph::remove_na(int u, int v){

    if(!BinarySearch3(update_out_edges[u], v)){
        cout<<u<<"->"<<v<<" not exist!!!"<<endl;
        return;
    }

    m--;

    update_in_num[v]--;
    update_out_num[u]--;
    update_in_edges[v].erase(std::remove(update_in_edges[v].begin(),update_in_edges[v].end(),u),update_in_edges[v].end());
    update_out_edges[u].erase(std::remove(update_out_edges[u].begin(),update_out_edges[u].end(),v),update_out_edges[u].end());

    if(BinarySearch3(update_out_edges[v], u)){
        delete_update_nei2_4(u,v);
        delete_update_nei2_4(v,u);
    }else{
        edge_m--;
        update_degree[u]--;
        update_degree[v]--;

        delete_update_nei1_4(u,v);
        delete_update_nei1_4(v,u);
    }
}

void Graph::delete_update_nei1_4(int u, int v){
        multimap<float, int, greater<float>> single = nbr_mp_c[u];
        nbr_mp_c[u].clear();

        for(multimap<float, int, greater<float>>::iterator it = single.begin();it!=single.end();++it){
            if(it->second == v){
                continue;
            }
            int all_c = 0;
            get_add_c4(u, it->second, all_c);
            float new_sim;
            if(BinarySearch3(update_out_edges[v], u) && BinarySearch3(update_out_edges[u], v)){
                new_sim = (float)(all_c+4)/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
            }else{
                new_sim = (float)(all_c)/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
            }
            update_nei_order_c(it->second, u, it->first, new_sim);
            nbr_mp_c[u].insert(make_pair(new_sim, it->second));
        }
        multimap<float, int, greater<float>> single2 = nbr_mp_f[u];
        nbr_mp_f[u].clear();
        for(multimap<float, int, greater<float>>::iterator it = single2.begin();it!=single2.end();++it){
            if(it->second == v){
                continue;
            }
            int all_f = 0;
            get_add_f4(u, it->second, all_f);
            
            float new_sim;
            if(BinarySearch3(update_out_edges[v], u) && BinarySearch3(update_out_edges[u], v)){
                new_sim = (float)(all_f+12)/(6*(pow(update_degree[u]*update_degree[it->second],0.5)));
            }else{
                new_sim = (float)(all_f + 4)/(6*(pow(update_degree[u]*update_degree[it->second],0.5)));
            }
            update_nei_order_f(it->second, u, it->first, new_sim);
            nbr_mp_f[u].insert(make_pair(new_sim, it->second));
        }
}

void Graph::delete_update_nei2_4(int u, int v){
        multimap<float, int, greater<float>> single = nbr_mp_c[u];
        nbr_mp_c[u].clear();

        for(multimap<float, int, greater<float>>::iterator it = single.begin();it!=single.end();++it){

            int all_c = 0;
            get_add_c4(u, it->second, all_c);
            float new_sim;
            if(BinarySearch3(update_out_edges[v], u) && BinarySearch3(update_out_edges[u], v)){
                new_sim = (float)(all_c+4)/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
            }else{
                new_sim = (float)(all_c)/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
            }
            update_nei_order_c(it->second, u, it->first, new_sim);
            nbr_mp_c[u].insert(make_pair(new_sim, it->second));
        }
        multimap<float, int, greater<float>> single2 = nbr_mp_f[u];
        nbr_mp_f[u].clear();
        for(multimap<float, int, greater<float>>::iterator it = single2.begin();it!=single2.end();++it){

            int all_f = 0;
            get_add_f4(u, it->second, all_f);
            
            float new_sim;
            if(BinarySearch3(update_out_edges[v], u) && BinarySearch3(update_out_edges[u], v)){
                new_sim = (float)(all_f+12)/(6*(pow(update_degree[u]*update_degree[it->second],0.5)));
            }else{
                new_sim = (float)(all_f + 4)/(6*(pow(update_degree[u]*update_degree[it->second],0.5)));
            }
            update_nei_order_f(it->second, u, it->first, new_sim);
            nbr_mp_f[u].insert(make_pair(new_sim, it->second));
        }
}

void Graph::get_add_c4(int u, int v, int &all_c){
    //int ui = 0, vi = 0, ul = update_out_num[u], vl = update_out_num[v];
    int ui = 0;
    int vi = 0;
    int vl = update_out_num[v];
    int ul = update_in_num[u];

    while(ui<ul && vi < vl){

        if(update_in_edges[u][ui] == update_out_edges[v][vi]){
            all_c++;
            ui++;
            vi++;
        }
        else if(update_in_edges[u][ui] < update_out_edges[v][vi]) ui++;
        else vi++;
    }
}
void Graph::get_add_f4(int u, int v, int &all_f){
    int ui = 0, vi = 0, ul = update_out_num[u], vl = update_out_num[v];

    while(ui<ul && vi < vl){

        if(update_out_edges[u][ui] == update_out_edges[v][vi]){
            all_f++;
            ui++;
            vi++;
        }
        else if(update_out_edges[u][ui] < update_out_edges[v][vi]) ui++;
        else vi++;
    }

    ui = 0;
    vi = 0;
    vl = update_in_num[v];
    while(ui<ul && vi < vl){

        if(update_out_edges[u][ui] == update_in_edges[v][vi]){
            all_f++;
            ui++;
            vi++;
        }
        else if(update_out_edges[u][ui] < update_in_edges[v][vi]) ui++;
        else vi++;
    }

    ui = 0;
    vi = 0;
    ul = update_in_num[u];
    while(ui<ul && vi < vl){

        if(update_in_edges[u][ui] == update_in_edges[v][vi]){
            all_f++;
            ui++;
            vi++;
        }
        else if(update_in_edges[u][ui] < update_in_edges[v][vi]) ui++;
        else vi++;
    }
}

void Graph::update_nei1_4(int u, int v){
    multimap<float, int, greater<float>> single = nbr_mp_c[u];
    nbr_mp_c[u].clear();
    for(multimap<float, int, greater<float>>::iterator it = single.begin();it!=single.end();++it){

        int all_c = 0;
        get_add_c4(u, it->second, all_c);
        
        float new_sim;
        if(BinarySearch3(update_out_edges[v], u) && BinarySearch3(update_out_edges[u], v)){
            new_sim = (float)(all_c+4)/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
        }else{
            new_sim = (float)(all_c)/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
        }
        update_nei_order_c(it->second, u, it->first, new_sim);
        nbr_mp_c[u].insert(make_pair(new_sim, it->second));
    }
    int all_c = 0;
    get_add_c4(u, v, all_c);
    float new_sim;
    if(BinarySearch3(update_out_edges[v], u) && BinarySearch3(update_out_edges[u], v)){
        new_sim = (float)(all_c+4)/(2*(pow(update_degree[u]*update_degree[v],0.5)));
    }else{
        new_sim = (float)(all_c)/(2*(pow(update_degree[u]*update_degree[v],0.5)));
    }
    nbr_mp_c[u].insert(make_pair(new_sim, v));

    multimap<float, int, greater<float>> single2 = nbr_mp_f[u];
    nbr_mp_f[u].clear();

    for(multimap<float, int, greater<float>>::iterator it = single2.begin();it!=single2.end();++it){
        int all_f = 0;
        get_add_f4(u, it->second, all_f);
        
        float new_sim;
        if(BinarySearch3(update_out_edges[v], u) && BinarySearch3(update_out_edges[u], v)){
            new_sim = (float)(all_f+12)/(6*(pow(update_degree[u]*update_degree[it->second],0.5)));
        }else{
            new_sim = (float)(all_f + 4)/(6*(pow(update_degree[u]*update_degree[it->second],0.5)));
        }

        update_nei_order_f(it->second, u, it->first, new_sim);
        nbr_mp_f[u].insert(make_pair(new_sim, it->second));

    }
    int all_f = 0;
    get_add_f4(u, v, all_f);
    if(BinarySearch3(update_out_edges[v], u) && BinarySearch3(update_out_edges[u], v)){
        new_sim = (float)(all_f+12)/(6*(pow(update_degree[u]*update_degree[v],0.5)));
    }else{
        new_sim = (float)(all_f + 4)/(6*(pow(update_degree[u]*update_degree[v],0.5)));
    }
    nbr_mp_f[u].insert(make_pair(new_sim, v));
    
}

void Graph::update_nei2_4(int u, int v){
        multimap<float, int, greater<float>> single = nbr_mp_c[u];
        nbr_mp_c[u].clear();
        for(multimap<float, int, greater<float>>::iterator it = single.begin();it!=single.end();++it){

            int all_c = 0;
            get_add_c4(u, it->second, all_c);
            float new_sim;
            if(BinarySearch3(update_out_edges[v], u) && BinarySearch3(update_out_edges[u], v)){
                new_sim = (float)(all_c+4)/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
            }else{
                new_sim = (float)(all_c)/(2*(pow(update_degree[u]*update_degree[it->second],0.5)));
            }
            update_nei_order_c(it->second, u, it->first, new_sim);
            nbr_mp_c[u].insert(make_pair(new_sim, it->second));
        }
        multimap<float, int, greater<float>> single2 = nbr_mp_f[u];
        nbr_mp_f[u].clear();

        for(multimap<float, int, greater<float>>::iterator it = single2.begin();it!=single2.end();++it){
            int all_f = 0;
            get_add_f4(u, it->second, all_f);
            float new_sim;
            if(BinarySearch3(update_out_edges[v], u) && BinarySearch3(update_out_edges[u], v)){
                new_sim = (float)(all_f+12)/(6*(pow(update_degree[u]*update_degree[it->second],0.5)));
            }else{
                new_sim = (float)(all_f + 4)/(6*(pow(update_degree[u]*update_degree[it->second],0.5)));
            }
            update_nei_order_f(it->second, u, it->first, new_sim);
            nbr_mp_f[u].insert(make_pair(new_sim, it->second));

        }
}

