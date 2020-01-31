#include "redisDb.h"
using namespace type ;

//判断当前操作键值存在不存在,找到并修改
int redisDb :: isExist(shared_ptr<Command>&cmds) {
    string cmd = cmds->cmd() ;
    //如果是set 命令
    int num = cmds->num() ;
    if(cmd == "set") {
        string k = cmds->keys(0).key(0) ;
        key ke(num, type::DB_STRING, k) ;
        auto res = db.find(ke) ;
        if(res == db.end()) {
            return 0 ;
        }
        res->second->setNum(cmds->num()) ;
        res->second->setValue(cmds->vals(0).val(0)) ;
    }
    if(cmd == "hset") {
        string k = cmds->keys(0).key(0) ;
        string kk = cmds->keys(1).key(0) ;
        string vv = cmds->vals(0).val(0) ;
        key ke(num, type::DB_HASH, k) ;
        auto res = db.find(ke) ;
        if(res == db.end()) {
            return 0 ;
        }
        int k_len = cmds->keys_size() ;
        for(int i=1; i<k_len; i++) {
            int lk = cmds->keys(i).key_size() ;
            int lv = cmds->vals(i-1).val_size() ;
            if(lk != lv) {
                return -1 ;
            }
            for(int j=0; j<lk; j++) {
                string kk = cmds->keys(i).key(j) ;
                string vv = cmds->vals(i-1).val(j) ;
                res->second->setValue(kk, vv.c_str()) ;
            }
        }
        res->second->setNum(cmds->num()) ;
    }
    if(cmd == "lpush") {
        ListObject lob = cmds->lob(0) ;
        string keys = lob.key() ;
        key ke(num, type::DB_LIST, keys) ;
        auto res = db.find(ke) ;
        if(res == db.end()) {
            return -1 ;
        }
        int len = lob.vals_size() ;
        for(int i=0; i<len; i++) {
            int size = lob.vals(i).val_size() ;
            for(int j=0; j<size; j++) {
                res->second->setValue(lob.vals(i).val(j)) ;
            }
        }
        num = res->second->objectSize() ;
        return num ;   
    }
    if(cmd == "zadd") {
        ListObject lob = cmds->lob(0) ;
        string keys = lob.key() ;
        key ke(num, type::SORT_SET, keys) ;
        auto res = db.find(ke) ;
        if(res == db.end()) {
            return -1 ;
        }
        Value val = lob.vals(0) ;
        string score = val.val(0);
        string value = val.val(1) ;
        res->second->setValue(score, value.c_str()) ;
    }
    return 1 ;
}

int redisDb::append(int num, int type, shared_ptr<dbObject>dob) {
    key k ;
    k.num = num ;
    k.type = type ;
    k.cmd = dob->getKey() ;
    auto s = db.find(k) ;
    if(s == db.end()) {
        db.insert({k, dob}) ;
    }
}

//遍历redis中的dbObject对象
shared_ptr<dbObject> redisDb :: getNextDb() {
    static auto res = db.begin();
    if(res == db.end() || !db.size()) { 
        res = db.begin() ;
        return nullptr ;
    }
    auto re = res ;
    shared_ptr<dbObject>dob = re->second ;
    res ++ ;
    int flag = 0 ;
    //判断当前超时
    long time = timer :: getCurTime() ;
    int end = dob->getEndTime() ;
    //超时值大于０的时候，判断超时,小于０表示是永久保存
    if(end > 0 && time > end) {
        flag = 1 ;
        db.erase(re) ;
    }
    return flag == 0 ? dob:nullptr ;
}

void redisDb :: append(shared_ptr<dbObject>rdb) {   
    //三元组确定对象
    key k  ;
    //数据库号
    k.num = rdb->getNum() ;
    //类型
    k.type = rdb->getType() ;
    //键值
    k.cmd = rdb->getKey() ;
    db.insert({k, rdb}); 
}

//查询数据库,get命令等
int redisDb :: queryDb(shared_ptr<Response>& res, shared_ptr<Command>& cmd) {
    //get查询
    string md = cmd->cmd() ;
    int num = cmd->num() ;/*
    cout << "当前数据库编号" << num << endl ;*/
    if(!strcasecmp(md.c_str(), "get")) {
        //在本数据库中找set对象并且查询的值
        string key ;
        //获取到键值
        key = cmd->keys(0).key(0) ;
        string r = findGetRequest(key, num) ;
        if(!r.size()) {
            res->set_reply("object is not find!") ;
        }
        else {
            res->set_reply("\""+r+"\"") ;
        }
    }

    if(!strcasecmp(md.c_str(), "hget")) {
        string value ;
        string key = cmd->keys(0).key(0) ;
        value = cmd->keys(0).key(1) ;
        string feild = findHgetRequest(key, value) ;
        if(feild == "") {
            res->set_reply("no the object!") ;
            return 1;
        }
        res->set_reply("\""+feild+"\"") ;
    }
    int flag = 0;
    if(!strcasecmp(md.c_str(), "lpop") || !strcasecmp(md.c_str(), "blpop")) {
        string key = cmd->lob(0).key() ;
        string val = findListRequest(key, num) ;
        if(!strcasecmp(md.c_str(), "blpop")) {
            flag = 1 ;
        }
        if(val.empty() && flag == 0) {
            res->set_reply("null") ;
            return 1 ;
        }
        //当blpop为空的时候,返回一个0
        if(val.empty()) {
            int a = 0 ;
            return a;
        }
        res->set_reply(val) ;
    }
    if(!strcasecmp(md.c_str(), "zrange")) {
        string key = cmd->lob(0).key() ;
        string val = findSortSetValue(cmd) ;
        if(val.empty()) {
           return -1 ; 
        }
        res->set_reply(val) ;
    }
    return 1 ;
}

string redisDb :: findSortSetValue(const shared_ptr<Command>& cmd) {
    key  ke ;
    ListObject lob = cmd->lob(0) ;
    ke.cmd = lob.key() ;
    string s = lob.vals(0).val(0) ;
    string e = lob.vals(0).val(1) ;
    string val = s+" "+e ;
    ke.num = cmd->num() ;
    ke.type = type::SORT_SET ;
    auto ret = db.find(ke) ;
    if(ret == db.end()) {
        cout << "没找到" << endl ;
        return "" ;
    }
    vector<string> res = ret->second->getValues(val) ;
    //将结果集合打包
    string ss = "" ;
    for(auto a : res) {
        ss+=a ;
        ss+='\n' ;
    }
    ss[ss.size()-1] = ' ';
    return ss ;
}

void redisDb :: processBlpop() {
    
}


string redisDb::findListRequest(const string k, const int num) {
    key ke ;
    ke.num = num ;
    ke.cmd = k ;
    ke.type = type::DB_LIST ;
    auto res = db.find(ke) ;
    if(res == db.end()) {
        return "" ;
    }
    if(res->second->objectSize() == 0) {
        //删除对象在数据库中的数据
        removeDataByKey(ke) ;
        return "" ;
    }
    string t= res->second->getValue() ;
    int len = res->second->objectSize() ;   
    return t+" "+to_string(len) ;
}

void redisDb :: removeDataByKey(key k) {
    for(auto s=db.begin(); s != db.end(); s++) {
        if(k == s->first){
            db.erase(s) ;
            break ;
        }
    }
}


string redisDb :: findHgetRequest(const string k, 
                                  const string feild) {
    key ke ;
    ke.num = num ;
    ke.type = DB_HASH ;
    ke.cmd = k ;
    auto res = db.find(ke) ;
    if(res == db.end()) {
        return "no the object!" ;
    }
    //获取过期时间
    long endTime = res->second->getEndTime() ;
    if(endTime != -1 && endTime < timer::getCurTime()) {
        db.erase(res) ;
        return "" ;
    }
    auto s = res->second->getValues(feild) ;
    return s[0] ;   
}
void redisDb :: print() {
    for(auto res=db.begin(); res != db.end(); res++) {
        res->second->print() ;
    }
}
//找相应的get请求键的值
string redisDb :: findGetRequest(const string k, const int num) {
    //查找
    key ke(num, DB_STRING, k) ;
    auto res= db.find(ke) ;
    if(res == db.end()) {
        return "" ;
    }
    else {
        long ti = res->second->getEndTime() ;
        if(ti != -1 && ti < timer::getCurTime()) {
            return "" ;
        } 
        return res->second->getValue() ; 
    }
}


