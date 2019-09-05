#pragma once
#include<iostream>
#include <string>
#include <list>
#include <vector>
#include <memory>
#include <map>
#include <set>
#include "msg.pb.h"
using namespace Messages ;
using namespace std ;

class redisDb ;
class dbObject ;
class setCommand ;
class factory ;
//数据库对象
class redisDb {
public :
    redisDb() {  }
    ~redisDb() {  } 
private :
    //数据库编号
    int num ;
    vector<shared_ptr<dbObject>> db ;
public :

    void append(shared_ptr<dbObject>rdb) { db.push_back(rdb); }
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
    //set操作
    virtual void setKey(string k) = 0;
    virtual void setValue(string v) = 0;
    virtual string getKey() = 0;
    virtual string getValue() = 0;
    //获取命令编号
    virtual int getNum() = 0 ;
private :
    //保存所有键的过期时间
    //redis对象管理dict对象
    //shared_ptr<dict>dic ;
};

//set对象
class setCommand : public dbObject {
public :
    setCommand() {}
    ~setCommand() {}
public :
    void setKey(string k) { this->key = k ; } 
    void setValue(string value)  { this->value = value ; }
    string  getName() { return key ; }
    string getKey() { return value ; }
    int getNum() { return num ; }
    string getValue() {}
public :
    map<string, long long> expire ;
    int num ;
    string key ;
    string value ;
   // map<string, string>common ;
} ;

class hsetCmd : public dbObject {
public :
    int num ;
    map<string, map<string, string>>hset ;
} ;
/*
//字典
class dict {
private:
    //redisDb中的list对象，键值对，键不能有重复
    map<string, list<string>> ls ;
    map<string, set<string>> comset ;
    //hash集合
    map<string, string>common ;
} ;*/
class factory {
public :
    static shared_ptr<dbObject> getObject(string cmd) {
        if(cmd == "set") {
            shared_ptr<setCommand> tmp(new setCommand) ;
            return tmp ;
        }   
        else {
            return nullptr ;
        }
    }
} ;

