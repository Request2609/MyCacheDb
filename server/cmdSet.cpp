#include "cmdSet.h"

vector<pair<int, std::shared_ptr<redisDb>>>*cmdSet::db = NULL;
int cmdSet :: REDIS_NUM = 16 ;
//初始化命令集
cmdSet :: cmdSet() {
    //申请16个数据库
    dbLs.reserve(REDIS_NUM) ;
    for(int i=0; i<16; i++) {
        dbLs.push_back({i ,std::make_shared<redisDb>(i)}) ;
    }
    db = &dbLs ;
} 

int cmdSet :: backUp() {
    if(aeEventloop::canSave == 1&& db!=NULL) {
        asyncSave() ;
    }
}

void cmdSet:: asyncSave() {
    int stat ;
    pid_t wpid ;
    //非阻塞wait
    wpid = waitpid(-1, &stat, WNOHANG) ;
    int fd = aeSocket::getWriteFd() ;
    aeEventloop::canSave = 0 ;    
    pid_t pid = fork() ;
    if(pid < 0) {
        return ;
    }
    if(pid == 0) {
        exit(0) ;
    }
    else {
        int ret = cmdCb::save(*db) ;
        //将持久化调用结果返回
        write(fd, &ret, sizeof(ret)) ;
        return ;
    }
}

//初始化命令表
int cmdSet :: initCmdCb() {
    //初始化set命令
    std::shared_ptr<redisCommand>tset = std::make_shared<redisCommand>("set", -3, "wm",  1, 1, 1, 0, 0) ;
    //函数指针不能作为构造函数参数
    tset->setCallBack(cmdCb :: setCmd) ;
    cmdList.insert(make_pair("set", tset)) ;

    std::shared_ptr<redisCommand>tget = std::make_shared<redisCommand>("get", -3, "wm",  1, 1, 1, 0, 0) ;
    tget->setCallBack(cmdCb :: getCmd) ;
    cmdList.insert(make_pair("get", tget)) ;
    //设置数据库命令
    std::shared_ptr<redisCommand>save = std::make_shared<redisCommand>("save", -3, "r", 1, 1, 1, 0, 0) ;
    save->setCallBack(cmdCb :: save) ;
    cmdList.insert({"save", save}) ;

    //设置hash命令的回调以及相关的信息
    std::shared_ptr<redisCommand>hashLs = std::make_shared<redisCommand>("hset", -3, "wm",  1, 1, 1, 0, 0) ;
    hashLs->setCallBack(cmdCb :: setHash) ;
    cmdList.insert(make_pair("hset", hashLs)) ;   

    //设置hget命令相关信息
    std::shared_ptr<redisCommand>hgetLs = std::make_shared<redisCommand>("hget", -3, "wm",  1, 1, 1, 0, 0) ;
    hgetLs->setCallBack(cmdCb :: setHget) ;
    cmdList.insert({"hget", hgetLs}) ;   

    std::shared_ptr<redisCommand>bgSave =std::make_shared<redisCommand>("bgsave", -3, "wm",  1, 1, 1, 0, 0) ;
    //和save一样调用相同的函数，操作文件
    bgSave->setCallBack(cmdCb :: save) ;
    cmdList.insert({"bgsave", bgSave}) ;   

    std::shared_ptr<redisCommand>lpush =std::make_shared<redisCommand>("lpush", -3, "wm",  1, 1, 1, 0, 0) ;
    lpush->setCallBack(cmdCb :: setLpush) ;
    cmdList.insert({"lpush", lpush}) ;   

    //从队列中弹出
    std::shared_ptr<redisCommand>lpop = std::make_shared<redisCommand>("lpop", -3, "wm",  1, 1, 1, 0, 0);
    lpop->setCallBack(cmdCb :: lPop) ;
    cmdList.insert({"lpop", lpop}) ;
    
    std::shared_ptr<redisCommand>blpop=std::make_shared<redisCommand>("blpop", -3, "wm",  1, 1, 1, 0, 0) ;
    blpop->setCallBack(cmdCb :: blPop) ;
    cmdList.insert({"blpop", blpop}) ;

    std::shared_ptr<redisCommand>zadd = std::make_shared<redisCommand>("zadd", -3, "wm",  1, 1, 1, 0, 0);
    zadd->setCallBack(cmdCb :: sortSetAdd) ;
    cmdList.insert({"zadd", zadd}) ;

    //查询有序集合的
    std::shared_ptr<redisCommand>zrange = std::make_shared<redisCommand>("zrange", -3, "wm",  1, 1, 1, 0, 0);
    zrange->setCallBack(cmdCb :: sortSetGetMember) ;
    cmdList.insert({"zrange", zrange}) ;

    //查询集合的
    std::shared_ptr<redisCommand>sadd = std::make_shared<redisCommand>("sadd", -3, "wm",  1, 1, 1, 0, 0);
    sadd->setCallBack(cmdCb :: setSetValue) ;
    cmdList.insert({"sadd", sadd}) ;

    //查询集合的
    std::shared_ptr<redisCommand>spop = std::make_shared<redisCommand>("zrange", -3, "wm",  1, 1, 1, 0, 0);
    spop->setCallBack(cmdCb :: sPop) ;
    cmdList.insert({"spop", spop}) ;
} 

//打印数据苦衷当前信息
void cmdSet :: print() {
    for(auto s : dbLs) {
        //数据库中的所有信息
       s.second->print() ;
    }
}

//初始化数据库
int cmdSet :: initRedis() {
   rdb :: initRedis(this) ;
}

int cmdSet:: findCmd(std::string cmd) {
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

void cmdSet :: addObjectToDb(int num, std::shared_ptr<dbObject>ob) {
    ob->setNum(num) ;
    for(auto s : dbLs) {
        if(s.first == num) {
            s.second->append(ob) ;
            break ;
        }
    }
    return ;
}

std::shared_ptr<redisDb> cmdSet :: getDB(int num) {
    //数据库为空
    int len = dbLs.size() ;
    for(int i=0; i<len; i++) {
        if(i == num) {
            return dbLs[i].second ;
        }
    }
    return nullptr ;
}

void cmdSet::checkAof() {
    recoverDb::recoverByLog(&dbLs) ;
}

int cmdSet :: append(std::shared_ptr<redisDb> db) {
    int num = db->getId() ;
    dbLs.push_back({num, db}) ;
    return 1 ;
}

int cmdSet :: redisCommandProc(int num, std::shared_ptr<Messages::Command>&cmd) {
    //创建一个响应
    response = std::make_shared<Messages::Response>() ;
    //根据数据库编号找到数据库
    std::shared_ptr<redisDb> wrdb = getDB(num) ;
    std::string cd = cmd->cmd() ;
    //不区分大小写a
    int a = 0 ;
    if(!strcasecmp(cd.c_str(), "set")) {
        saveTimerHandle::countModify() ;
        //调用命令对应的函数
        a = cmdList[cd]->cb(wrdb, cmd, response) ;
        //处理失败
    }
    if(!strcasecmp(cd.c_str(), "lpop")) {
        saveTimerHandle::countModify() ;
        a = cmdList[cd]->cb(wrdb, cmd, response) ;
    }

    //lpush命令
    if(!strcasecmp(cd.c_str(), "lpush")) {
        saveTimerHandle::countModify() ;
        a = cmdList[cd]->cb(wrdb, cmd, response) ;   
    }

    if(!strcasecmp(cd.c_str(), "get")) {
        a = cmdList[cd]->cb(wrdb, cmd, response) ;
    }

    if(!strcasecmp(cd.c_str(), "save")) {
        //将数据库遍历一遍
        a = cmdList[cd]->saveCb(dbLs) ;
        if(a < 0) {
            response->set_reply("SAVE FAIL!") ;
        }
        else {
            response->set_reply("OK") ;
        }
    }

    if(!strcasecmp(cd.c_str(), "zadd")) {
        saveTimerHandle::countModify() ;
        a = cmdList[cd]->cb(wrdb, cmd, response) ;
        if(a > 0) {
            response->set_reply("1") ;
        }
        else {
            response->set_reply("SAVE FAIL") ;
        }
    }
    //哈希
    if(!strcasecmp(cd.c_str(), "hset")) {
        saveTimerHandle::countModify() ;
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
        if(a < 0) {
            response->set_reply("FAIL") ;
        }
    }

    //fork进程
    //异步持久化
    if(!strcasecmp(cd.c_str(), "bgsave")) {
        a = 1 ;
        if(aeEventloop::canSave == 0) {
            response->set_reply("OK") ;
        }
        //异步持久化
        cmdSet::asyncSave() ;
    }

    if(!strcasecmp(cd.c_str(), "blpop")) {
        std::string aa = "blpop" ;
        //给a设置一个特殊值
        a = cmdList[aa]->cb(wrdb, cmd, response) ;
        //接收a的值进行判断
        if(a == 0) {
            return 0 ;
        }
    }
    if(!strcasecmp(cd.c_str(), "zrange")) {
        //给a设置一个特殊值
        std::string aa = cd.c_str() ;
        a = cmdList[aa]->cb(wrdb, cmd, response) ;
        //接收a的值进行判断
        if(a == 0) {
            return 0 ;
        }
    }
    if(!strcasecmp(cd.c_str(), "sadd")) {
        saveTimerHandle::countModify() ;
         //给a设置一个特殊值
        std::string aa = cd.c_str() ;
        a = cmdList[aa]->cb(wrdb, cmd, response) ;
        if(a > 1) {
            response->set_reply("1") ;
        }
        //接收a的值进行判断
        if(a == 0) {
            return 0 ;
        }
    }

    if(!strcasecmp(cd.c_str(), "spop")) {
        saveTimerHandle::countModify() ;
        //给a设置一个特殊值
        std::string aa = cd.c_str() ;
        a = cmdList[aa]->cb(wrdb, cmd, response) ;
        //接收a的值进行判断
        if(a == 0) {
            return 0 ;
        }
    }

    if(a < 0) {
        response->set_reply("FAIL") ;
        return PROCESSERROR ;
    }
    return SUCESS ;
}
int redisCommand :: cb(std::shared_ptr<redisDb>&db, std::shared_ptr<Messages::Command>&wcmd, std::shared_ptr<Messages::Response>& res) { 
    if(callBack == nullptr) {
        return -1;
    }
    return callBack(db, wcmd, res) ; 
}
