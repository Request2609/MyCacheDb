#pragma once
#include<iostream>
#include <string>
#include <list>
#include <vector>
#include <string.h>
#include <memory>
#include <map>
#include <set>
#include "timer.h"
#include "msg.pb.h"
#include "timer.h"

using namespace Messages ;
using namespace std ;


namespace type {
    //该键值是string 使用get　set 方法获取
    const int DB_STRING = 3 ;
} 

class redisDb ;
class dbObject ;
class setCommand ;
class factory ;
//数据库对象
class redisDb {
public :
    redisDb(int id) : num(id) {  }
    ~redisDb() {  } 
private :
    //数据库编号
    int num ;
    //string的存储方式,第一个元素是数据库编号，第二个是命令类别，第三个是具体的数据对象，三元组确定对象
    map<pair<pair<int, int>, string>, shared_ptr<dbObject>> db ;
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
    string findGetRequest(string name, int num) ;
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
    virtual void setValue(string v) = 0;
    virtual void setName(string name) = 0;
    virtual void setNum(int num) = 0;
    virtual string  getName() = 0;
    virtual string getKey() = 0;
    virtual int getNum() = 0 ;
    virtual string getValue() = 0;
    //获取命令编号
private :
    //保存所有键的过期时间
    //redis对象管理dict对象
};

//set对象
class setCommand : public dbObject {
public :
    setCommand() {
        timeout = -1 ;
    }
    ~setCommand() {}
public :
    void print() ;
    long long getEndTime() { return timeout ; }
    void setEndTime(long long e) {this->timeout = e ;} 
    int getType() { return type ; }
    void setType(int type) { this->type = type ; }
    void setKey(string k) { this->key = k ; } 
    void setValue(string value)  { this->value = value ; }
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

/*
class getCommand : public dbObject {
    
} ;

class hsetCmd : public dbObject {
public :
    int num ;
    map<string, map<string, string>>hset ;
    };
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
        if(!strcasecmp(cmd.c_str(), "set")) {
            shared_ptr<setCommand> tmp(new setCommand) ;
            return tmp ;
        }   
        else {
            return nullptr ;
        }
    }
} ;

