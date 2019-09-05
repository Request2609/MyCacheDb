#include "cmdSet.h"
#include "enum.h"

int cmdSet:: findCmd(string cmd) {
    if(cmdList.find(cmd) == cmdList.end()) {
        return NOTFOUND ;
    }   
    else {
        return FOUND ;
    }
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

//设置set命令的处理
int cmdSet :: setCmd(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&cmd) {
    int num = cmd->keys(0).key_size() ;
    //键的数量不是1,错误的
    if(num != 1) {
        return KEYINVALID ;
    }
    
    //键存不存在,存在的话,就地修改,返回1, 不存在返回0
    int ret = isKeyExist(wcmd, cmd) ;
    //没找到键值
    if(ret == 0) {
        shared_ptr<dbObject>se = factory::getObject("set");
        se->setKey(cmd->keys(0).key(0)) ;
        se->setValue(cmd->vals(0).val(0)) ;
        //将数据存在相应的数据库中
        wcmd->append(se) ;
    }
    return SUCESS ;
}

int cmdSet :: isKeyExist(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&cmd) {
       int ret = wcmd->isExist(cmd) ;
       return ret ;
}

int cmdSet :: redisCommandProc(int num, shared_ptr<Command>&cmd) {
    //数据库编号
    shared_ptr<redisDb> wrdb = getDB(num) ;
    string cd = cmd->cmd() ;
    //不区分大小写
    if(!strcasecmp(cd.c_str(), "set")) {
        //调用命令对应的函数
        int ret = cmdList[cd]->cb(wrdb, cmd) ;
        //处理失败
        if(ret < 0) {
            return PROCESSERROR ;
        }
        return SUCESS ;
    }
    //get 命令
    if(!strcasecmp(cd.c_str(), "get")) {
        int ret = cmdList[cd].cb(wrdb, cmd) ;
        if(ret < 0) {
            return PROCESSERROR ;
        }
        return SUCESS ;
    }
    //下面是一系列命令
}
