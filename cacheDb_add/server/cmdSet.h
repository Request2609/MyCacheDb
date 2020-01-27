#pragma once
#include <string>
#include <functional>
#include <map>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <vector>
#include <sys/mman.h>

#include "redisDb.h"
#include "ThreadPool.h"
#include "aeEvent.h"
#include "msg.pb.h"
#include "rdb.h"
#include "cmdCb.h"
#include "logRecord.h"


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
class cmdCb ;
class Flag ;
class logRecord ; 

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
    void setCallBack(saveCall save) { 
        this->save = save ;}
    void setCallBack(call cb) { 
        this->callBack = cb ; 
    }
    int append(int num, int type, shared_ptr<dbObject>ptr) ;
    string getName() { return name ; }
    //函数指针，指向命令的具体实现
private :
    call callBack ;
    saveCall save;
private :
    string stringRes ;
    string name ;
    //参数数量限制,用于验证参数是否正确
    int arity ;
    //命令的权限位，只读的r，不确定的输出R，在redis加载数据的时候使用l
    string flags ;
    //只有在你要用复杂的逻辑去告诉Redis哪个参数才是真正的key的时候才需要。
    //命令的度量项，有数据库设置，初始化为０
    int firstKey ;
    int lastKey ;
    int keyStep ;
    //微妙,时间
    long long microSecond, calls ;
} ;


//命令集合
class cmdSet {
    friend class logRecord ;
public:
    //设置命令的回调函数
    cmdSet() ;
    ~cmdSet() {}
public :
    static int REDIS_NUM;
    int initCmdCb() ;
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
    int append(shared_ptr<redisDb> db) ;
    void print() ;
    void saveToFrozenRedis(int num) ;
    void sigintProcess() ;
private:
    shared_ptr<rdb> save ;
    //回复，响应
    shared_ptr<Response> response ;
    //数据库,键值是数据库编号码,之后数据库对象
    vector<pair<int, shared_ptr<redisDb>>>dbLs ;
    vector<pair<int, shared_ptr<redisDb>>>frozenDbLs ;
    //命令名称，命令类型
    map<string, shared_ptr<redisCommand>> cmdList ;  
    shared_ptr<threadPool> pool ;
};

