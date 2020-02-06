#pragma once
#include<iostream>
#include <string>
#include <list>
#include <vector>
#include <string.h>
#include <memory>
#include <unordered_map>
#include <cstdarg>
#include <map>

#include "recoverDb.h"
#include "aofRecord.h"
#include "dbObject.h"
#include "msg.pb.h"

using namespace Messages ;
using namespace std ;

class recoverDb ;

namespace type {
    //该键值是string 使用get　set 方法获取
    const int DB_STRING = 3 ;
    const int DB_HASH = 4 ;
    const int DB_LIST = 5 ;
    const int SORT_SET = 6 ;
    const int SET_SET = 7 ;
} 

class redisDb ;
class dbObject ;
class strings;
class hashSet ;
class factory ;
class hashFunc ;
class equalFunc ;
class key ; 
class hashFunc ;
class equalFunc ;
class aofRecord ;

class key {
public :
    key() {}
    key(int n, int t, string c) : num(n), type(t), cmd(c) {}
    bool operator==(const key&k) {
        return num==k.num&&cmd==k.cmd&&type==k.type ;
    }
    bool operator>(const key&k) {
        return timer>k.timer?1:0 ;
    }
    bool operator<(const key&k) {
        return timer>k.timer?1:0 ;
    }
public :
    int timer ;
    int num ;
    int type ;
    string cmd ;
} ;

class hashFunc {
public :
    std :: size_t operator()(const key& k) const{
        return (((hash<int>()(k.num)<<1)^(hash<string>()(k.cmd)<<1))>>1)^(hash<int>()(k.type) >> 1) ;
    }   
} ;

class equalFunc {
public :
    bool operator() (const key& k1, const key& k2) const{
        return k1.num == k2.num && k1.type == k2.type && k1.cmd == k2.cmd ;
    }   
} ;

//数据库对象
class redisDb {
public :
    redisDb(int id) : num(id) {  }
    ~redisDb() {  } 
private :
    //数据库编号
    int num ;
    unordered_map<key, shared_ptr<dbObject>, hashFunc, equalFunc> db ;
public :
    //往数据库中写数据
    int initDb() ;
    //获取当前数据库id
    //判断数据库是否为空
    int append(int num, int type, shared_ptr<dbObject>dob) ;
    int isEmpty() { return db.size() ; }
    void print() ;
    void setId(int id) { this->num = id ; }
    int getId() { return num ; }
    shared_ptr<dbObject>getNextDb() ;
    string findGetRequest(const string name, const int num) ;
    string findHgetRequest(const string key, const string field) ;
    string findListRequest(const string key, const int num) ;
    string findSortSetValue(const shared_ptr<Command>&cmd) ;
    string findSetRequest(const string key, const int num );
    void processBlpop() ;
    int queryDb(shared_ptr<Response>& res, shared_ptr<Command>& cmd) ;
    void append(shared_ptr<dbObject>rdb) ;
    int getSize() { return db.size(); }
        //命令键
    int isExist(shared_ptr<Command>&cmds) ;
    //获取key
    //删除当前数据库中的一个对象
    void removeDataByKey(key k) ;
} ;

