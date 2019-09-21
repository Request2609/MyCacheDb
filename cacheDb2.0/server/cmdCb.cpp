#include "cmdCb.h"
//hash函数
int cmdCb :: setHash(shared_ptr<redisDb>&wcmd, 
                     shared_ptr<Command>&tmp, 
                     shared_ptr<Response>& res) {               
    int ret = wcmd->isExist(tmp) ;
    if(ret == 0) {
        res->set_reply("OK") ;
        return 1 ;
    }
    shared_ptr<dbObject>se = factory::getObject("hset");
    if(se == nullptr) {
        return -1 ;
    }
    se->setKey(tmp->keys(0).key(0)) ;
    int k_len = tmp->keys_size() ;
    int v_len = tmp->vals_size() ;
    for(int i=1; i<k_len; i++) {
        int lk = tmp->keys(i).key_size() ;
        int lv = tmp->vals(i-1).val_size() ;
        if(lk != lv) {
            return -1 ;
        }
        for(int j=0; j<lk; j++) {
            string kk = tmp->keys(i).key(j) ;
            string vv = tmp->vals(i-1).val(j) ;
            se->setValue(kk, vv.c_str()) ;
        }
    }
    se->setNum(tmp->num()) ;
    se->setName("hset") ;
    se->setType(type::DB_HASH) ;
    wcmd->append(se) ;
    res->set_reply("OK") ;
}

int cmdCb :: setHget(shared_ptr<redisDb>&db, 
                     shared_ptr<Command>&tmp, 
                     shared_ptr<Response>& res) {
    db->queryDb(res, tmp) ;
    return 1 ;
}

int cmdCb :: isKeyExist(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&cmd) {
       int ret = wcmd->isExist(cmd) ;
       return ret ;
}

int cmdCb :: rdbSave(vector<pair<int, shared_ptr<redisDb>>>& dls) {
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

//设置set命令的处理
int cmdCb :: setCmd(shared_ptr<redisDb>&wcmd, 
                    shared_ptr<Command>&cmd, 
                    shared_ptr<Response>& res) {
    int len = cmd->keys(0).key_size() ;
    //键的数量不是1,错误的
    if(len != 1) {
        return 3 ;
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
    return 4 ;
}

//get命令处理函数
int cmdCb :: getCmd(shared_ptr<redisDb>&db, 
                    shared_ptr<Command>&tmp, 
                    shared_ptr<Response>& res) {
    //在指定数据库中找相应的值
    db->queryDb(res, tmp) ;
    return 1 ;
}
