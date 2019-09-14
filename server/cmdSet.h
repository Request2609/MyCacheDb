#pragma once
#include <string>
#include <functional>
#include <map>
#include <fstream>
#include <vector>
#include "redisDb.h"
#include "aeEvent.h"
#include "msg.pb.h"
#include "rdb.h"

class rdb ;
using namespace std  ;
using namespace Messages ;

enum {
    NOTFOUND =   1,
    FOUND = 2,
    KEYINVALID = 3,
    SUCESS = 4,
    PROCESSERROR = -1 
} ; 
//暂时没用
typedef int *redisGetKeysProc();
class rdb ;
class cmdSet ;
class redisDb ;
class dbObject ;
class factory ;
class redisCommand ;

class redisCommand {
    //该命令的的处理函数
    typedef function<int(shared_ptr<redisDb>&, shared_ptr<Command>&, shared_ptr<Response>&)>call ;
    typedef function<int(vector<pair<int, shared_ptr<redisDb>>>&db)> saveCall;
public :
    redisCommand(string name, int arity, string flag,  
                 int fir, int last, int keyStep, long long msecond, long long calls) {
            this->name = name ;
            this->arity = arity ;
            this->flags = flag ;
 ///           this->callBack = move(callBack) ;
            this->firstKey = fir ;
            this->lastKey = last ;
            this->keyStep = keyStep ;
            this->microSecond = msecond ;
            this->calls = calls ;
    } 
    ~redisCommand() {}
public :
    int saveCb(vector<pair<int, shared_ptr<redisDb>>>&db) { return save(db) ; } 
    int cb(shared_ptr<redisDb>&db, shared_ptr<Command>&wcmd, shared_ptr<Response>& res) ;
    void setCallBack(saveCall save) { this->save = save ;}
    void setCallBack(call cb) { this->callBack = cb ; }
    //函数指针，指向命令的具体实现
private :
    saveCall save;
    call callBack ;
private :
    string stringRes ;
    string name ;
    //参数数量限制,用于验证参数是否正确
    int arity ;
    //命令的权限位，只读的r，不确定的输出R，在redis加载数据的时候使用l
    string flags ;
    //只有在你要用复杂的逻辑去告诉Redis哪个参数才是真正的key的时候才需要。
///:    redisGetKeysProc*getKeys ;
    //命令的度量项，有数据库设置，初始化为０
    int firstKey ;
    int lastKey ;
    int keyStep ;
    //微妙,时间
    long long microSecond, calls ;
} ;

//命令集合
class cmdSet {
public:
    //设置命令的回调函数
    cmdSet() {
        //申请16个数据库
        dbLs.reserve(16) ;
        for(int i=0; i<16; i++) {
            dbLs.push_back({i ,shared_ptr<redisDb>(new redisDb(i))}) ;
        }
        //初始化set命令
        shared_ptr<redisCommand>tset(new redisCommand("set", -3, "wm",  1, 1, 1, 0, 0)) ;
        //函数指针不能作为构造函数参数
        tset->setCallBack(setCmd) ;
        cmdList.insert(make_pair("set", tset)) ;

        shared_ptr<redisCommand>tget(new redisCommand("get", -3, "wm",  1, 1, 1, 0, 0)) ;
        tget->setCallBack(getCmd) ;
        cmdList.insert(make_pair("get", tget)) ;
        //设置数据库命令
        shared_ptr<redisCommand>bgsave(new redisCommand("bgsave", -3, "r", 1, 1, 1, 0, 0)) ;
        bgsave->setCallBack(rdbSave) ;
        cmdList.insert({"bgsave", bgsave}) ;
    } 

    ~cmdSet() {}
public :
    int getSize() { return dbLs.size() ; }
    int expend(int num) ;
    int countRedis() ;
    int initRedis() ;
    int redisCommandProc(int num, shared_ptr<Command>& cmd) ;
    void addObjectToDb(int num, shared_ptr<dbObject>ob) ;
    shared_ptr<redisDb> getDB(int num) ;
    //扩大数据库
    //返回命令集合
    int findCmd(string cmd) ;  
    shared_ptr<Response> getResponse() { return response ; }
    int bgSaveCmd(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&tmp, shared_ptr<Response>& res) ;
    int append(shared_ptr<redisDb> db) ;
public :
    static int rdbSave(vector<pair<int, shared_ptr<redisDb>>>&dbLs) ;
    static int isKeyExist(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&cmd) ;
    static int setCmd(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&tmp, shared_ptr<Response>& res);
    static int getCmd(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&tmp, shared_ptr<Response>& res) ;
private:
    shared_ptr<rdb> save ;
    //回复，响应
    shared_ptr<Response> response ;
    //数据库,键值是数据库编号码,之后数据库对象
    vector<pair<int, shared_ptr<redisDb>>>dbLs ;
    //命令名称，命令类型
    map<string, shared_ptr<redisCommand>> cmdList ;  
};

