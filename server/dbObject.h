#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include<stdarg.h>
#include<string.h>
#include <list>
#include "rbTree.h"

class lsObject ;
class rb_tree ;

class dbObject {
public:
    dbObject() {}
    virtual ~dbObject() {}
public :
    long timer ;
    virtual void print() = 0;
    //set操作
    virtual long long  getEndTime() = 0 ;
    virtual void setEndTime(long long e) = 0 ;
    virtual int getType() = 0 ;
    virtual void setType(int type) = 0;
    virtual void setKey(std::string k) = 0;
    virtual void setValue(std::string v, ...) = 0;
    virtual void setName(std::string name) = 0;
    virtual void setNum(int num) = 0;
    virtual std::string  getName() = 0;
    virtual std::string getKey() = 0;
    virtual int getNum() = 0 ;
    virtual std::string getValue() = 0;
    virtual std::vector<std::string> getValues(const std::string s) = 0;
    virtual int objectSize() = 0 ;
};

//set对象
class setContain : public dbObject {
public :
    setContain();
    ~setContain() ; 
public :
    std::vector<std::string> getValues(const std::string s) {}
    void print() {}
    long long getEndTime() { return timeout ; }
    void setEndTime(long long e) {this->timeout = e ;}
    int getType() { return type ; }
    void setType(int type) { this->type = type ; }
    void setKey(std::string k) { this->key = k ; }
    void setValue(std::string value, ...)  ;
    void setName(std::string name) { this->name = name ; }
    void setNum(int num) { this->num = num ; }
    std::string  getName() { return name ; }
    std::string getKey() { return key ; }
    int getNum() { return num ; }
    std::string getValue() ;
    int objectSize() { return 1 ;}

private :
    //类型
    std::set<std::string> ls ;
    int type ;
    std::string name ;
    long long timeout ;
    //当前设置的超时时间
    //所在数据库编号
    int num ;
    //命令键值
    std::string key ;
    std::string value ;
} ;


//set对象
class strings : public dbObject {
public :
    strings() {
        timeout = -1 ;
    }
    ~strings() {}
public :
    std::vector<std::string> getValues(const std::string s) {}
    void print() ;
    long long getEndTime() { return timeout ; }
    void setEndTime(long long e) {this->timeout = e ;} 
    int getType() { return type ; }
    void setType(int type) { this->type = type ; }
    void setKey(std::string k) { this->key = k ; } 
    void setValue(std::string value, ...)  { this->value =  value ; }
    void setName(std::string name) { this->name = name ; }
    void setNum(int num) { this->num = num ; }
    std::string  getName() { return name ; }
    std::string getKey() { return key ; }
    int getNum() { return num ; }
    std::string getValue() { return value ; }
    int objectSize() { return 1 ;}
public :
    //类型
    int type ;
    //命令名称
    std::string name ;
    long long timeout ;
    //当前设置的超时时间
    map<std::string, long long> expire ;
    //所在数据库编号
    int num ;
    //命令键值
    std::string key ;
    std::string value ;
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
    void setKey(std::string k) {key = k ;}
    void setValue(std::string v, ...) ;
    void setName(std::string name) {
        this->name =name ;
    }
    void setNum(int num) { this->num = num ; }
    std::string  getName() { return "" ;}
    std::string getKey() { return key ; }
    int getNum() { return num ; }
    std::string getValue() ;
    std::vector<std::string> getValues(const std::string s) ;
    int objectSize() { return ls.size() ; }
private :
    long long timeout ;
    int count ;
    std::string key ;
    std::list<std::string> ls ;
    int num ;
    int type ;
    std::string name ;
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
    std::vector<std::string> getValues(const std::string s) ;
    long long getEndTime() { return timeout ; }
    void setEndTime(long long e) {this->timeout = e ;} 
    int getType() { return type ; }
    void setType(int type) { this->type = type ; }
    void setKey(std::string k) { this->key = k ; } 
    void setValue(std::string key, ...) ;
    void setName(std::string name) { this->name = name ; }
    void setNum(int num) { this->num = num ; }
    std::string  getName() { return name ; }
    std::string getKey() { return key ; }
    int getNum() { return num ; }
    std::string getValue() ;
    int objectSize() { return values.size() ;}
public :
    //类型
    int type ;
    //命令名称
    std::string name ;
    long long timeout ;
    //当前设置的超时时间
    std::map<std::string, long long> expire ;
    //所在数据库编号
    int num ;
    //命令键值
    std::string key ;
    std::map<std::string, std::string>values ;
} ;

//有序set对象
class sortSet : public dbObject {
public :
    sortSet() {
        timeout = -1 ;
    }
    ~sortSet() {}
public :
    std::vector<std::string> getValues(const std::string s) ;
    void print() {}
    long long getEndTime() { return timeout ; }
    void setEndTime(long long e) {this->timeout = e ;}
    int getType() { return type ; }
    void setType(int type) { this->type = type ; }
    void setKey(std::string k) { this->key = k ; }
    void setValue(std::string value, ...)  ;
    void setName(std::string name) { this->name = name ; }
    void setNum(int num) { this->num = num ; }
    std::string  getName() { return name ; }
    std::string getKey() { return key ; }
    int getNum() { return num ; }
    std::string getValue() { value = ""; return value ; }
    int objectSize() { return 1 ;}
public :
    std::shared_ptr<rb_tree>rbt ;
    //类型
    int type ;
    std::string name ;
    long long timeout ;
    //当前设置的超时时间
    //所在数据库编号
    int num ;
    //命令键值
    std::string key ;
    std::string value ;
} ;


class factory {
public :
    static std::shared_ptr<dbObject> getObject(std::string cmd) {
        if(!strcasecmp(cmd.c_str(), "set")) {
            std::shared_ptr<strings> tmp(new strings) ;
            return tmp ;
        }   
        if(!strcasecmp(cmd.c_str(), "hset")) {
            std::shared_ptr<hashSet> tmp = std::make_shared<hashSet>(-1) ;
            return tmp ;       
        }
        if(!strcasecmp(cmd.c_str(), "lpush")) {
            std::shared_ptr<lsObject> tmp = std::make_shared<lsObject>() ;
            return tmp ;
        }
        if(!strcasecmp(cmd.c_str(), "zadd")) {
            std::shared_ptr<sortSet>tmp = std::make_shared<sortSet>() ;
            return tmp ;
        }
        if(!strcasecmp(cmd.c_str(), "sadd")) {
            std::shared_ptr<setContain>tmp = std::make_shared<setContain>() ;
            return tmp ;
        }
        else {
            return nullptr ;
        }
    }
} ;


