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
    if(cmd == "lpop") {
        ListObject lob = cmds->lob() ;
        string keys = lob.key() ;
        key ke(num, type::DB_LIST, keys) ;
        auto res = db.find(ke) ;
        if(res == db.end()) {
            return -1 ;
        }
        string val = res->second->getValue() ; 
        int num = res->second->objectSize() ;
    }
    return 1 ;
}

//遍历redis中的dbObject对象
shared_ptr<dbObject> redisDb :: getNextDb() {
    cout << "开始获取！"<< endl ;
    static auto res = db.begin();
    cout << "获取下一个数据库！" << endl ;
    if(res == db.end() || !db.size()) { 
        res = db.begin() ;
        cout << "获取结束！"<< endl ;
        return nullptr ;
    }
    auto re = res ;
    shared_ptr<dbObject>dob = re->second ;
    res ++ ;
    cout << "获取成功！"  << endl ;
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
    int num = cmd->num() ;
    cout << "数据库中的数据信息------>" << endl ;
    print() ;
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
        cout << "get命令处理完成" << endl ;
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
        string value ;
        string key = cmd->lob().key() ;
        ListObject lb = cmd->lob() ;
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
    return 1 ;
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
        return "" ;
    }
    string t= res->second->getValue() ;
    int len = res->second->objectSize() ;   
    return t+" "+to_string(len) ;
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


