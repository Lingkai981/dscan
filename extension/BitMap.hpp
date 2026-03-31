
#ifndef BitMap_hpp
#define BitMap_hpp

#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
using namespace std;
class BitMap{

    int capacity_;
    char* bm_;
    int unit_bits_;

public:
    BitMap();
    BitMap(int size);
    void init(int size);
    ~BitMap();
    int set(int pos, int val);
    int get(int pos);

};
#endif /* BitMap_hpp */




