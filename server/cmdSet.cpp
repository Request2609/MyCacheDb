#include "cmdSet.h"
#include "enum.h"
#include "rdb.h"

int cmdSet :: rdbSave(vector<pair<int, shared_ptr<redisDb>>>& dls) {
    //存储各个数据库文件的文件
    ofstream out(".rdb/.redis_fileName", ios::out|ios::binary|ios::trunc) ;
    //遍历数据库进行保存
    //先创建存储数据库文件名的文件
    char fileName[100] ;
    bzero(fileName, sizeof(fileName)) ;
    int ret  = 1 ;
    for(auto p : dls) {
        if(!p.second->getSize()) {
            continue ;
        }
        //根据编号构造文件名
        sprintf(fileName, ".rdb/.db_%d", p.first) ;
        //将数据库文件名存在文件中
        out << fileName << endl ;
        //将数据库和文件名传进rdb持久化程序进行处理
        ret = rdb :: save(p.second, fileName) ;
        if(ret < 0) {
            return -1 ;
        }
    }
    out.close() ;
    return ret ;
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

//设置set命令的处理
int cmdSet :: setCmd(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&cmd, shared_ptr<Response>& res) {
    int len = cmd->keys(0).key_size() ;
    //键的数量不是1,错误的
    if(len != 1) {
        return KEYINVALID ;
    }
    int num = cmd->num() ;
    //键存不存在,存在的话,就地修改,返回1, 不存在返回0
    int ret = isKeyExist(wcmd, cmd) ;
    //没找到键值
    if(ret == 0) {
        shared_ptr<dbObject>se = factory::getObject("set");
        se->setType(type::DB_STRING) ;
        se->setName("set") ;
        se->setNum(num) ;
        se->setKey(cmd->keys(0).key(0)) ;
        se->setValue(cmd->vals(0).val(0)) ;
        //将数据存在相应的数据库中
        wcmd->append(se) ;      
    }
    res->set_reply("OK") ;
    return SUCESS ;
}

int cmdSet :: append(shared_ptr<redisDb> db) {
    int num = db->getId() ;
    dbLs.push_back({num, db}) ;
    return 1 ;
}

//get命令处理函数
int cmdSet :: getCmd(shared_ptr<redisDb>&db, shared_ptr<Command>&tmp, shared_ptr<Response>& res) {
    //在指定数据库中找相应的值
    db->queryDb(res, tmp) ;
    return 1 ;
}

int cmdSet :: isKeyExist(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&cmd) {
       int ret = wcmd->isExist(cmd) ;
       return ret ;
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
        //将数据库遍历一遍
        a = cmdList[cd]->saveCb(dbLs) ;
        if(a < 0) {
            response->set_reply("SAVE FAIL!") ;
        }
        else {
            response->set_reply("OK") ;
        }
    }
    //下面是一系列命令
    if(a < 0) {
        return PROCESSERROR ;
    }
    return SUCESS ;
}

int redisCommand :: cb(shared_ptr<redisDb>&db, shared_ptr<Command>&wcmd, shared_ptr<Response>& res) { 
    return callBack(db, wcmd, res) ; 
}
