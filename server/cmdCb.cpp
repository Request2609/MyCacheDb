#include "cmdCb.h"
int cmdCb:: blPop(std::shared_ptr<redisDb>&db, 
                  std::shared_ptr<Messages::Command>&tmp, 
                  std::shared_ptr<Messages::Response>&res) {
    int aa = db->queryDb(res, tmp) ;
    return aa;
}

//设置push命令
int cmdCb :: setLpush(std::shared_ptr<redisDb>&wcmd, 
                      std::shared_ptr<Messages::Command>&tmp, 
                      std::shared_ptr<Messages::Response>& res) {
    int ret = wcmd->isExist(tmp) ;
    if(ret != -1) {
        char buf[50] ;
        sprintf(buf, "\n%d\n", ret) ;
        res->set_reply(buf) ;
        return ret ;
    }
    auto ptr = aofRecord::getLogObject() ;
    std::string s =" "+std::to_string(tmp->num()) +" "+"lpush";
    std::shared_ptr<dbObject>se = factory::getObject("lpush");
    ListObject lob = tmp->lob(0) ;
    s+=" "+lob.key() ;
    se->setKey(lob.key()) ;
    int len = lob.vals_size() ;
    for(int i=0; i<len; i++) {
        int size = lob.vals(i).val_size() ;
        for(int j=0; j<size; j++) {
            se->setValue(lob.vals(i).val(j)) ; 
            s+=" "+lob.vals(i).val(j) ;
        }
    }
    se->setNum(tmp->num()) ;
    se->setType(type::DB_LIST_) ;
    int num = se->objectSize() ;
    //设置成功，返回当前队列中的元素个数
    res->set_reply(std::to_string(num)) ;
    wcmd->append(se) ;
    ptr->record(s.c_str()) ;
    return 1 ;
 }

int cmdCb :: setHash(std::shared_ptr<redisDb>&wcmd, 
                     std::shared_ptr<Messages::Command>&tmp, 
                     std::shared_ptr<Messages::Response>& res) {               
    int ret = isKeyExist(wcmd, tmp) ;
    if(ret == 1) {
        res->set_reply("OK") ;
        return 1 ;
    }

    auto ptr = aofRecord::getLogObject() ;
    std::string s ="" ;
    std::shared_ptr<dbObject>se = factory::getObject("hset");
    if(se == nullptr) {
        return -1 ;
    }
    s+=" "+std::to_string(tmp->num()) +" "+"hset";
    se->setKey(tmp->keys(0).key(0)) ;
    int k_len = tmp->keys_size() ;
    for(int i=1; i<k_len; i++) {
        int lk = tmp->keys(i).key_size() ;
        int lv = tmp->vals(i-1).val_size() ;
        if(lk != lv) {
            return -1 ;
        }
        for(int j=0; j<lk; j++) {
            std::string kk = tmp->keys(i).key(j) ;
            string vv = tmp->vals(i-1).val(j) ;
            s+=" "+kk+" "+vv ;
            se->setValue(kk, vv.c_str()) ;
        }
    }
    se->setNum(tmp->num()) ;
    se->setName("hset") ;
    se->setType(type::DB_HASH_) ;
    wcmd->append(se) ;
    ptr->record(s.c_str()) ;
    res->set_reply("OK") ;
}

int cmdCb :: lPop(std::shared_ptr<redisDb>& db, 
         std::shared_ptr<Messages::Command>&tmp, 
         std::shared_ptr<Messages::Response>& res) {
    db->queryDb(res, tmp) ;
    return 1 ;
}

int cmdCb :: setHget(std::shared_ptr<redisDb>&db, 
                     std::shared_ptr<Messages::Command>&tmp, 
                     std::shared_ptr<Messages::Response>& res) {
    db->queryDb(res, tmp) ;
    return 1 ;
}

int cmdCb :: isKeyExist(std::shared_ptr<redisDb>&wcmd, 
                        std::shared_ptr<Messages::Command>&cmd) {
       int ret = wcmd->isExist(cmd) ;
       return ret ;
}

int cmdCb :: save(vector<pair<int, std::shared_ptr<redisDb>>>& dls) {
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
    return 1 ;
}

//设置set命令的处理
int cmdCb :: setCmd(std::shared_ptr<redisDb>&wcmd, 
                    std::shared_ptr<Messages::Command>&cmd, 
                    std::shared_ptr<Messages::Response>& res) {
    int n = cmd->keys_size() ;
    if(n == 0) {
        res->set_reply("FAIL") ;
        return -1 ;
    }
    auto ptr = aofRecord::getLogObject() ;
    std::string s ="" ;
    //键的数量不是1,错误的
    int num = cmd->num() ;
    //键存不存在,存在的话,就地修改,返回1, 不存在返回0
    int ret = isKeyExist(wcmd, cmd) ;
    //没找到键
    if(ret == 0) {
        s+=" "+std::to_string(cmd->num())+" " +"set" ;
        std::shared_ptr<dbObject>se = factory::getObject("set");
        se->setType(type::DB_STRING) ;
        se->setName("set") ;
        se->setNum(num) ;
        se->setKey(cmd->keys(0).key(0)) ;
        se->setValue(cmd->vals(0).val(0)) ;
        s+=" " +cmd->vals(0).val(0) ;
        ptr->record(s.c_str()) ;
        //将数据存在相应的数据库中
        wcmd->append(se) ;     
    }
    res->set_reply("OK") ;
    if(ret < 0) {
        res->set_reply("FAIL") ;
    }
    return 1 ;
}

int cmdCb::sortSetAdd(std::shared_ptr<redisDb>&wcmd, std::shared_ptr<Messages::Command>&tmp, std::shared_ptr<Messages::Response>&res) {
    if(wcmd->isExist(tmp) > 0) {
        return 1 ;
    }
    auto ptr = aofRecord::getLogObject() ;
    std::string s ="" ;
    std::shared_ptr<dbObject>se = factory::getObject("zadd") ;
    ListObject lob = tmp->lob(0) ;
    s+=" "+std::to_string(tmp->num())+ "sadd "+lob.key()+" "+lob.vals(0).val(0)+" "+lob.vals(0).val(1);
    se->setKey(lob.key()) ;
    se->setValue(lob.vals(0).val(0), lob.vals(0).val(1).c_str()) ;
    se->setNum(tmp->num());
    se->setType(type::SORT_SET) ;
    wcmd->append(tmp->num(), type::SORT_SET, se) ;
    ptr->record(s.c_str()) ;
    return 1 ;     
}

int cmdCb :: setSetValue(std::shared_ptr<redisDb>&wcmd, std::shared_ptr<Messages::Command>&tmp, std::shared_ptr<Messages::Response>&res) {
    if(wcmd->isExist(tmp)>0) {
        return 1 ;
    }
    auto ptr = aofRecord::getLogObject() ;
    std::string s ="" ;
    auto se = factory::getObject("sadd") ;
    ListObject lob = tmp->lob(0) ;
    se->setKey(lob.key()) ;
    s+=" "+std::to_string(tmp->num())+" "+lob.key() ;
    Value val = lob.vals(0) ;
    int size = val.val_size() ;
    for(int i=0; i<size;i++) {
       se->setValue(val.val(i)) ;
       s+=" "+val.val(i) ;
    }
    ptr->record(s.data()) ;
    se->setType(type::SET_SET) ;
    se->setNum(tmp->num()) ;
    int ret = wcmd->append(tmp->num(), type::SET_SET, se);
    res->set_reply("OK") ;
    return ret;
}

int cmdCb :: sPop(std::shared_ptr<redisDb>&wcmd, 
                  std::shared_ptr<Messages::Command>&tmp, 
                  std::shared_ptr<Messages::Response>&res) {
    int ret = wcmd->queryDb(res, tmp) ;
    return ret ;
}

int cmdCb::sortSetGetMember(std::shared_ptr<redisDb>&wcmd, 
                            std::shared_ptr<Messages::Command>&tmp, 
                            std::shared_ptr<Messages::Response>&res) {
    int ret = wcmd->queryDb(res, tmp) ;
    return ret ;
}

//get命令处理函数
int cmdCb :: getCmd(std::shared_ptr<redisDb>&db, 
                    std::shared_ptr<Messages::Command>&tmp, 
                    std::shared_ptr<Messages::Response>& res) {
    //在指定数据库中找相应的值
    db->queryDb(res, tmp) ;
    return 1 ;
}
