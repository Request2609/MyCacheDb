#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <string>
#include <unordered_map>
#include "msg.pb.cpp"
#include "threadPool.h"

using namespace std ;
using namespace Messages ;

class threadPool ;
//创建记录aof的键值
struct aofKey {
    long num ; //序列号
    string cmd ; //命令
} ;

class aofEqualFunc {
public :
    bool operator() (const aofKey& k1, const aofKey& k2) const{
        return k1.num == k2.num && k1.cmd == k2.cmd ;
    }
} ;

class aofHashFunc {
public :
    size_t operator() (const aofKey& k) const  {
        return (((hash<int>()(k.num) << 1)^(hash<string>()(k.cmd) << 1)>>1)) ;
    }
} ;


class aofOperation {
public:
    aofOperation() {}
    ~aofOperation() {}
public :
    static unsigned long seq ;
    static void stringData(const int num, const shared_ptr<Command>&cmd) ; 
    static void hsetData(const int num, const shared_ptr<Command>&cmd) ;
    static shared_ptr<threadPool> pool ;
    //记录用户写操作
    //aofKey记录数据库编号和写操作，vector记录相关的键值
    //map对应键和后面的一系列值
    //值使用map是因为可以为数据根据键来排序
    static unordered_map<aofKey, map<string, vector<pair<string, string>>>, aofHashFunc, aofEqualFunc> aofHsetRcd ;
    static unordered_map<aofKey, map<string, string>, aofHashFunc, aofEqualFunc>aofStrRcd ;
};

