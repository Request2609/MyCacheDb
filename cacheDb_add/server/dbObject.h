#pragma once
#include <iostream>
#include <strings.h>
#include <vector>
#include <map>
#include <memory>
#include<stdarg.h>
#include <string.h>
#include <list>
#include "rbTree.h"
using namespace std ;

class lsObject ;
class rb_tree ;

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
    virtual int objectSize() = 0 ;
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
    int objectSize() { return 1 ;}
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

class lsObject : public dbObject{
public :
    lsObject() {
        timeout = -1 ;
        count = -1 ;
    }
    ~lsObject() {}
public :
    void print() ;
    //set操作
    long long  getEndTime() { return timeout ; }
    void setEndTime(long long e) {}
    int getType()  { return type; }
    void setType(int type) { this->type = type ; }
    void setKey(string k) {key = k ;}
    void setValue(string v, ...) ;
    void setName(string name) {
        this->name =name ;
    }
    void setNum(int num) { this->num = num ; }
    string  getName() { return "" ;}
    string getKey() { return key ; }
    int getNum() { return num ; }
    string getValue() ;
    vector<string> getValues(const string s) ;
    int objectSize() { return ls.size() ; }
private :
    long long timeout ;
    int count ;
    string key ;
    list<string> ls ;
    int num ;
    int type ;
    string name ;
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
    int objectSize() { return values.size() ;}
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

//有序set对象
class setSet : public dbObject {
public :
    setSet() {
        timeout = -1 ;
    }
    ~setSet() {}
public :
    vector<string> getValues(const string s) ;
    void print() {}
    long long getEndTime() { return timeout ; }
    void setEndTime(long long e) {this->timeout = e ;}
    int getType() { return type ; }
    void setType(int type) { this->type = type ; }
    void setKey(string k) { this->key = k ; }
    void setValue(string value, ...)  ;
    void setName(string name) { this->name = name ; }
    void setNum(int num) { this->num = num ; }
    string  getName() { return name ; }
    string getKey() { return key ; }
    int getNum() { return num ; }
    string getValue() { value = ""; return value ; }
    int objectSize() { return 1 ;}
public :
    shared_ptr<rb_tree>rbt ;
    //类型
    int type ;
    string name ;
    long long timeout ;
    //当前设置的超时时间
    //所在数据库编号
    int num ;
    //命令键值
    string key ;
    string value ;
} ;


//有序set对象
class sortSet : public dbObject {
public :
    sortSet() {
        timeout = -1 ;
    }
    ~sortSet() {}
public :
    vector<string> getValues(const string s) ;
    void print() {}
    long long getEndTime() { return timeout ; }
    void setEndTime(long long e) {this->timeout = e ;}
    int getType() { return type ; }
    void setType(int type) { this->type = type ; }
    void setKey(string k) { this->key = k ; }
    void setValue(string value, ...)  ;
    void setName(string name) { this->name = name ; }
    void setNum(int num) { this->num = num ; }
    string  getName() { return name ; }
    string getKey() { return key ; }
    int getNum() { return num ; }
    string getValue() { value = ""; return value ; }
    int objectSize() { return 1 ;}
public :
    
    //类型
    int type ;
    string name ;
    long long timeout ;
    //当前设置的超时时间
    //所在数据库编号
    int num ;
    //命令键值
    string key ;
    string value ;
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
        if(!strcasecmp(cmd.c_str(), "lpush")) {
            shared_ptr<lsObject> tmp = make_shared<lsObject>() ;
            return tmp ;
        }
        if(!strcasecmp(cmd.c_str(), "zadd")) {
            shared_ptr<sortSet>tmp = make_shared<sortSet>() ;
            return tmp ;
        }
        if(!strcasecmp(cmd.c_str(), "sadd")) {
            shared_ptr<setSet>tmp = make_shared<setSet>() ;
            return tmp ;
        }
        else {
            return nullptr ;
        }
    }
} ;


