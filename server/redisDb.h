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
#include <time.h>

#include "lru.h"
#include "recoverDb.h"
#include "aofRecord.h"
#include "dbObject.h"
#include "msg.pb.h"


class recoverDb ;

namespace type {
    //该键值是string 使用get　set 方法获取
    const int DB_STRING = 3 ;
    const int DB_HASH = 4 ;
    const int DB_LIST = 5 ;
    const int SORT_SET = 6 ;
    const int SET_SET = 7 ;
} 

class curTimer ;
class simpleLru ;
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
    key(int n, int t, std::string c) : num(n), type(t), cmd(c) {}
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
    std::string cmd ;
} ;

class hashFunc {
public :
    size_t operator()(const key& k) const{
        return (((std::hash<int>()(k.num)<<1)^
            (std::hash<std::string>()(k.cmd)<<1))>>1)^
            (std::hash<int>()(k.type) >> 1) ;
    }   
} ;

class equalFunc {
public :
    bool operator() (const key& k1, const key& k2) const{
        return k1.num == k2.num && 
            k1.type == k2.type && k1.cmd == k2.cmd ;
    }   
} ;

//数据库对象
class redisDb {
public :
    redisDb(int id) : num(id)  { 
        srand((int)time(0)) ;
        MAX_SIZE = 1024*1024*1024 ;
    }
    ~redisDb() {  } 
private :
    std::vector<key>kls ;
    long MAX_SIZE ;
    //数据库编号
    int num ;
    std::unordered_map<key, std::shared_ptr<dbObject>, hashFunc, equalFunc> db ;
public :
    //往数据库中写数据
    int initDb() ;
    //获取当前数据库id
    //判断数据库是否为空
    int append(int num, int type, std::shared_ptr<dbObject>dob) ;
    long getMaxMem() { 
        return MAX_SIZE; 
    }
    bool isFull(long size) { 
        return size>MAX_SIZE ; 
    }
    int isEmpty() { 
        return db.size() ; 
    }
    void print() ;
    void setId(int id) { 
        this->num = id ; 
    }
    int getId() { 
        return num ; 
    }
    std::shared_ptr<dbObject>getNextDb() ;
    std::string findGetRequest(const std::string name, const int num) ;
    std::string findHgetRequest(const std::string key, const std::string field) ;
    std::string findListRequest(const std::string key, const int num) ;
    key getRandomKey() ;
    std::string findSortSetValue(const std::shared_ptr<Messages::Command>&cmd) ;
    std::string findSetRequest(const std::string key, const int num );
    void processBlpop() ;
    int queryDb(std::shared_ptr<Messages::Response>& res, 
                std::shared_ptr<Messages::Command>& cmd) ;
    void append(std::shared_ptr<dbObject>rdb) ;
    int getSize() { 
        return db.size(); 
    }
        //命令键
    int isExist(std::shared_ptr<Messages::Command>&cmds) ;
    //获取key
    //删除当前数据库中的一个对象
    void removeDataByKey(key k) ;
} ;

class curTimer{
public:
    static long curTime() ;
} ;
