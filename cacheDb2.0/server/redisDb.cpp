#include "redisDb.h"
using namespace type ;

//判断当前操作键值存在不存在,找到并修改
int redisDb :: isExist(shared_ptr<Command>&cmds) {
    string cmd = cmds->cmd() ;
    //如果是set 命令
    if(cmd == "set") {
        int num = cmds->num() ;
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
        int num = cmds->num() ;
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
    k.num = rdb->getNum() ;
    k.type = rdb->getType() ;
    k.cmd = rdb->getKey() ;
    db.insert({k, rdb}); 
}

//查询数据库,get命令等
void redisDb :: queryDb(shared_ptr<Response>& res, shared_ptr<Command>& cmd) {
    //get查询
    string md = cmd->cmd() ;
    int num = cmd->num() ;
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
            return ;
        }
        res->set_reply("\""+feild+"\"") ;
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

void hashSet :: print() {
    for(auto s : values) {
        cout << num <<"==============>  " <<  s.first <<"  -------  "<< s.second <<"    " <<  type<< endl ;
    }
}

void strings :: print() {
    cout << type<< "    " << num<< "   "<< key << "    "<< value << endl ;
}


vector<string> hashSet :: getValues(const string s) {
    vector<string>ls ;
    auto val = values.find(s) ;
    if(val == values.end()) {
        ls.push_back("hashset has no this feild!") ;
        return ls ;
    }
    ls.push_back(values[s]) ;
    return ls ;
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


string hashSet :: getValue() {
    static int flag = 0 ;
    static auto iter = values.begin() ;
    if(flag == 0) {
        iter = values.begin() ;
    }
    if(iter == values.end()) {
        iter = values.begin() ;
        flag = 0 ;
        return "" ;
    }
    flag = 1 ;
    string vv = iter->first + "\r\n" + iter->second ;
    iter++ ;
    return vv ;
}

//字符串的形式传进来
void hashSet :: setValue(string value, ...) {
    string key = value ;
    va_list va ;
    va_start(va, value) ;
    char* val = va_arg(va, char*) ;
    values[key] = val ;
    va_end(va) ;
}
