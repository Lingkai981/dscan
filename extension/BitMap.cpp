
#include "BitMap.hpp"

BitMap::BitMap(){
    capacity_ = 0;
    unit_bits_ = 0;
    bm_ = NULL;
}

BitMap::BitMap(int size){

    // cout<<13.331<<endl;
    capacity_ = size;
    // cout<<13.332<<endl;
    unit_bits_ = sizeof(char);
    // cout<<13.333<<endl;
    int windows_size = (size-1)/unit_bits_+1;
    // cout<<13.334<<endl;
    bm_ = new char[windows_size];
    // cout<<13.335<<endl;
    memset(bm_, 0, unit_bits_ * windows_size);
    // cout<<13.336<<endl;
}

BitMap::~BitMap(){
    if(bm_ != NULL) delete[] bm_;
}

void BitMap::init(int size){
    cout<<13.331<<endl;
    capacity_ = size;
    cout<<13.332<<endl;
    unit_bits_ = sizeof(char);
    cout<<13.333<<endl;
    int windows_size = (size-1)/unit_bits_+1;
    cout<<13.334<<endl;
    bm_ = new char[windows_size];
    cout<<13.335<<endl;
    memset(bm_, 0, unit_bits_ * windows_size);
    cout<<13.336<<endl;
}

int BitMap::set(int pos, int val){
    if(pos >= capacity_){
        printf("Position overflow\n");
        return -1;
    }
    // if(pos == 403) cout<<2222222<<endl;
    int addr = pos/unit_bits_;
    int addr_offset = pos%unit_bits_;
    int reverse_move = unit_bits_ - addr_offset -1;


    unsigned char tmp = 0x1 << reverse_move;

    if(val) bm_[addr] |= tmp;
    else{

        bm_[addr] &= ~tmp;
    }

    return 1;
}

int BitMap::get(int pos){
    if(pos >= capacity_){
        printf("Position overflow\n");
        return -1;
    }
    int addr = pos/unit_bits_;
    int addr_offset = pos%unit_bits_;
    int reverse_move = unit_bits_ - addr_offset -1;

    unsigned char tmp = 0x1 << reverse_move;

    return (bm_[addr] & tmp) >> reverse_move;

}



