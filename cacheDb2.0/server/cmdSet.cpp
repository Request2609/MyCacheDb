#include "cmdSet.h"
#include "enum.h"
#include "rdb.h"

//初始化命令集
cmdSet :: cmdSet() {
    //申请16个数据库
    dbLs.reserve(16) ;
    for(int i=0; i<16; i++) {
        dbLs.push_back({i ,shared_ptr<redisDb>(new redisDb(i))}) ;
    }
} 

int cmdSet :: initCmdCb() {
    //初始化set命令
    shared_ptr<redisCommand>tset(new redisCommand("set", -3, "wm",  1, 1, 1, 0, 0)) ;
    //函数指针不能作为构造函数参数
    tset->setCallBack(cmdCb :: setCmd) ;
    cmdList.insert(make_pair("set", tset)) ;

    shared_ptr<redisCommand>tget(new redisCommand("get", -3, "wm",  1, 1, 1, 0, 0)) ;
    tget->setCallBack(cmdCb :: getCmd) ;
    cmdList.insert(make_pair("get", tget)) ;
    //设置数据库命令
    shared_ptr<redisCommand>bgsave(new redisCommand("bgsave", -3, "r", 1, 1, 1, 0, 0)) ;
    bgsave->setCallBack(cmdCb :: rdbSave) ;
    cmdList.insert({"bgsave", bgsave}) ;
    //设置hash命令的回调以及相关的信息
    shared_ptr<redisCommand>hashLs(new redisCommand("hset", -3, "wm",  1, 1, 1, 0, 0)) ;
    hashLs->setCallBack(cmdCb :: setHash) ;
    cmdList.insert(make_pair("hset", hashLs)) ;   
    //设置hget命令相关信息
    shared_ptr<redisCommand>hgetLs(new redisCommand("hget", -3, "wm",  1, 1, 1, 0, 0)) ;
    hgetLs->setCallBack(cmdCb :: setHget) ;
    cmdList.insert({"hget", hgetLs}) ;   
}

//初始化数据库
int cmdSet :: initRedis() {
   rdb :: initRedis(this) ;
}

int cmdSet:: findCmd(string cmd) {
    if(cmdList.find(cmd) == cmdList.end()) {
        return NOTFOUND ;
    }   
    else {
        return FOUND ;
    }
}

int cmdSet :: expend(int num) {
    dbLs.reserve(num+1) ;   
}

int cmdSet :: countRedis() {
    return dbLs.size() ;
}

void cmdSet :: addObjectToDb(int num, shared_ptr<dbObject>ob) {
    ob->setNum(num) ;
    for(auto s : dbLs) {
        if(s.first == num) {
            s.second->append(ob) ;
            break ;
        }
    }
    return ;
}

shared_ptr<redisDb> cmdSet :: getDB(int num) {
    //数据库为空
    int len = dbLs.size() ;
    for(int i=0; i<len; i++) {
        if(i == num) {
            return dbLs[i].second ;
        }
    }
    return nullptr ;
}


int cmdSet :: append(shared_ptr<redisDb> db) {
    int num = db->getId() ;
    dbLs.push_back({num, db}) ;
    return 1 ;
}

int cmdSet :: redisCommandProc(int num, shared_ptr<Command>&cmd) {
    //创建一个响应
    response = make_shared<Response>() ;
    //根据数据库编号找到数据库
    shared_ptr<redisDb> wrdb = getDB(num) ;
    string cd = cmd->cmd() ;
    //不区分大小写a
    int a ;
    if(!strcasecmp(cd.c_str(), "set")) {
        //调用命令对应的函数
        a = cmdList[cd]->cb(wrdb, cmd, response) ;
        //处理失败
    }
    //get 命令
    if(!strcasecmp(cd.c_str(), "get")) {
        a = cmdList[cd]->cb(wrdb, cmd, response) ;
    }
    if(!strcasecmp(cd.c_str(), "bgsave")) {
        cout << "bgsave 命令！" << endl ;
        //将数据库遍历一遍
        a = cmdList[cd]->saveCb(dbLs) ;
        if(a < 0) {
            response->set_reply("SAVE FAIL!") ;
        }
        else {
            response->set_reply("OK") ;
        }
    }
    //哈希
    if(!strcasecmp(cd.c_str(), "hset")) {
        //hash表操作
        a = cmdList[cd]->cb(wrdb, cmd, response) ;
        if(a < 0) {
            response->set_reply("FAIL") ;
            return 1 ;
        }
        response->set_reply("OK") ;
    }
    if(!strcasecmp(cd.c_str(), "hget")) {
        int  a = cmdList[cd]->cb(wrdb, cmd, response) ;
        cout << cmdList[cd]->getName() << endl ;

        if(a < 0) {
            response->set_reply("FAIL") ;
        }
    }
    //下面是一系列命令
    if(a < 0) {
        return PROCESSERROR ;
    }
    return SUCESS ;
}

int redisCommand :: cb(shared_ptr<redisDb>&db, shared_ptr<Command>&wcmd, shared_ptr<Response>& res) { 
    if(callBack == nullptr) {
        return -1;
    }
    return callBack(db, wcmd, res) ; 
}
