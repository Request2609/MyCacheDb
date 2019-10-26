#include "cmdSet.h"
#include "enum.h"
#include "rdb.h"

//先创建四个线程
shared_ptr<threadpool> cmdSet :: pool = make_shared<threadpool>(4);

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
    shared_ptr<redisCommand>tset(new redisCommand("set", -3, "w",  1, 1, 1, 0, 0)) ;
    //函数指针不能作为构造函数参数
    tset->setCallBack(cmdCb :: setCmd) ;
    cmdList.insert(make_pair("set", tset)) ;

    shared_ptr<redisCommand>tget(new redisCommand("get", -3, "r",  1, 1, 1, 0, 0)) ;
    tget->setCallBack(cmdCb :: getCmd) ;
    cmdList.insert(make_pair("get", tget)) ;
    //设置数据库命令
    shared_ptr<redisCommand>save(new redisCommand("save", -3, "r", 1, 1, 1, 0, 0)) ;
    save->setCallBack(cmdCb :: save) ;
    cmdList.insert({"save", save}) ;
    //设置hash命令的回调以及相关的信息
    shared_ptr<redisCommand>hashLs(new redisCommand("hset", -3, "w",  1, 1, 1, 0, 0)) ;
    hashLs->setCallBack(cmdCb :: setHash) ;
    cmdList.insert(make_pair("hset", hashLs)) ;   
    //设置hget命令相关信息
    shared_ptr<redisCommand>hgetLs(new redisCommand("hget", -3, "r",  1, 1, 1, 0, 0)) ;
    hgetLs->setCallBack(cmdCb :: setHget) ;
    cmdList.insert({"hget", hgetLs}) ;   
    shared_ptr<redisCommand>bgSave(new redisCommand("bgsave", -3, "w",  1, 1, 1, 0, 0)) ;
    //和save一样调用相同的函数，操作文件
 //   bgSave->setCallBack(cmdCb :: save) ;
    cmdList.insert({"bgsave", bgSave}) ;   
}

//初始化数据库
    static shared_ptr<threadpool>pool ; 

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
    //判断异步写是否异常的标志
    static future<int>res ;
    //判断是否为第一次进行持久化操作
    static int isFirst = 0 ;
    
    //操作是写操作的话，就将命令持久化到aof文件
    //创建一个响应
    response = make_shared<Response>() ;
    //根据数据库编号找到数据库
    shared_ptr<redisDb> wrdb = getDB(num) ;
    string cd = cmd->cmd() ;
    //不区分大小写a
    int a ;
    //写操作的进行aof记录
    if(!strcasecmp(cd.c_str(), "set")) {
        //调用命令对应的函数
        //先将写的信息存在aof缓存中
        aofOperation :: getData(num, cmd) ;
        a = cmdList[cd]->cb(wrdb, cmd, response) ;
        //处理失败
    }
    //get 命令
    if(!strcasecmp(cd.c_str(), "get")) {
        a = cmdList[cd]->cb(wrdb, cmd, response) ;
    }
    if(!strcasecmp(cd.c_str(), "save")) {
        //表明上一次进行了异步持久化
        if(isFirst == 1) { 
            //获取上一次持久化的结果
            if(res.get() < 0) {
                response.set_reply("PREVIOUS SAVE FAILED, CONTINUE TO TRY AGAIN！") ; 
                isFirst = 0 ;
            }
        } 
        else {
            //将数据库遍历一遍
            a = cmdList[cd]->saveCb(dbLs) ;
            if(a < 0) {
                response->set_reply("SAVE FAIL!") ;
            }
            else {
                response->set_reply("OK") ;
            }
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
        aofOperation :: hsetData(num, cmd) ;
        response->set_reply("OK") ;
    }
    if(!strcasecmp(cd.c_str(), "hget")) {
        int  a = cmdList[cd]->cb(wrdb, cmd, response) ;
        if(a < 0) {
            response->set_reply("FAIL") ;
        }
    }

    if(!strcasecmp(cd.c_str(), "bgsave")) {
        string aa = "bgsave" ;
        //如果是第一次进行持久化
        if(isFirst == 0) {
            //开线程进行处理
            res = cmdSet :: pool->commit(cmdCb::save, dvLs) ;
            //将isFirst标记为1
            isFirst = 1 ;
        }
        else {
            int ff = res.get() ;
            //判断上一次持久是否成功
            if(ff == -1) {
                //如果上一次持久化不成功，则拒绝本次持久化操作
                response->set_reply("SAVE FAILED!") ;
                isFirst = 0 ;
            }   
        }
    }
    if(a < 0) {
        response->set_reply("FAIL") ;
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
