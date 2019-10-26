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

#include "timer.h"
#include "msg.pb.h"
#include "timer.h"

using namespace Messages ;
using namespace std ;
using namespace google ;


namespace type {
    //该键值是string 使用get　set 方法获取
    const int DB_STRING = 3 ;
    const int DB_HASH = 4 ;
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

class key {
public :
    key() {}
    key(int n, int t, string c) : num(n), type(t), cmd(c) {}
public :
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
    //为了查找快速，这里使用hashmap维护每个缓存数据库
    unordered_map<key, shared_ptr<dbObject>, hashFunc, equalFunc> db ;
public :
    //往数据库中写数据
    int initDb() ;
    //获取当前数据库id
    //判断数据库是否为空
    int isEmpty() { return db.size() ; }
    void print() ;
    void setId(int id) { this->num = id ; }
    int getId() { return num ; }
    shared_ptr<dbObject>getNextDb() ;
    string findGetRequest(const string name, const int num) ;
    string findHgetRequest(const string key, const string field) ;
    void queryDb(shared_ptr<Response>& res, shared_ptr<Command>& cmd) ;
    void append(shared_ptr<dbObject>rdb) ;
    int getSize() { return db.size(); }
        //命令键
    int isExist(shared_ptr<Command>&cmds) ;
    //获取key
    //删除当前数据库中的一个对象
    void del() {
    }
} ;

class dbObject {
public:
    dbObject() {}
    virtual ~dbObject() {}
public :
    virtual void print() = 0;
    //set操作
    virtual long long  getEndTime() = 0 ;
    virtual void setEndTime(long long e) = 0 ;
    virtual int getType() = 0 ;
    virtual void setType(int type) = 0;
    virtual void setKey(string k) = 0;
    virtual void setValue(string v, ...) = 0;
    virtual void setName(string name) = 0;
    virtual void setNum(int num) = 0;
    virtual string  getName() = 0;
    virtual string getKey() = 0;
    virtual int getNum() = 0 ;
    virtual string getValue() = 0;
    virtual vector<string> getValues(const string s) = 0;
    //获取命令编号
};

//set对象
class strings : public dbObject {
public :
    strings() {
        timeout = -1 ;
    }
    ~strings() {}
public :
    vector<string> getValues(const string s) {}
    void print() ;
    long long getEndTime() { return timeout ; }
    void setEndTime(long long e) {this->timeout = e ;} 
    int getType() { return type ; }
    void setType(int type) { this->type = type ; }
    void setKey(string k) { this->key = k ; } 
    void setValue(string value, ...)  { this->value =  value ; }
    void setName(string name) { this->name = name ; }
    void setNum(int num) { this->num = num ; }
    string  getName() { return name ; }
    string getKey() { return key ; }
    int getNum() { return num ; }
    string getValue() { return value ; }
public :
    //类型
    int type ;
    //命令名称
    string name ;
    long long timeout ;
    //当前设置的超时时间
    map<string, long long> expire ;
    //所在数据库编号
    int num ;
    //命令键值
    string key ;
    string value ;
} ;

class hashSet : public dbObject {
public :
    hashSet() {}
    hashSet(int t) {
        timeout = t ;
    }
    ~hashSet() {}
public :
    void print() ;
    vector<string> getValues(const string s) ;
    long long getEndTime() { return timeout ; }
    void setEndTime(long long e) {this->timeout = e ;} 
    int getType() { return type ; }
    void setType(int type) { this->type = type ; }
    void setKey(string k) { this->key = k ; } 
    void setValue(string key, ...) ;
    void setName(string name) { this->name = name ; }
    void setNum(int num) { this->num = num ; }
    string  getName() { return name ; }
    string getKey() { return key ; }
    int getNum() { return num ; }
    string getValue() ;
public :
    //类型
    int type ;
    //命令名称
    string name ;
    long long timeout ;
    //当前设置的超时时间
    map<string, long long> expire ;
    //所在数据库编号
    int num ;
    //命令键值
    string key ;
    map<string, string>values ;
} ;

class factory {
public :
    static shared_ptr<dbObject> getObject(string cmd) {
        if(!strcasecmp(cmd.c_str(), "set")) {
            shared_ptr<strings> tmp(new strings) ;
            return tmp ;
        }   
        if(!strcasecmp(cmd.c_str(), "hset")) {
            shared_ptr<hashSet> tmp = make_shared<hashSet>(-1) ;
            return tmp ;       
        }
        else {
            return nullptr ;
        }
    }
} ;

