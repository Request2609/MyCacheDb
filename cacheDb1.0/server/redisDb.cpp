#include "redisDb.h"
using namespace type ;

//判断当前操作键值存在不存在
int redisDb :: isExist(shared_ptr<Command>&cmds) {
    string cmd = cmds->cmd() ;
    //如果是set 命令
    if(cmd == "set") {
        int num = cmds->num() ;
        string key = cmds->keys(0).key(0) ;
        auto res = db.find({{num, type::DB_STRING}, key}) ;
        if(res != db.end()) {
            return 1 ;
        }
    }
    return 0 ;
}

//遍历redis中的dbObject对象
shared_ptr<dbObject> redisDb :: getNextDb() {
    static auto res = db.begin();
    auto re = res ++ ;
    if(re == db.end() || !db.size()) { 
        return nullptr ;
    }
    int flag = 0 ;
    shared_ptr<dbObject>dob = re->second ;
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
    db.insert({{{rdb->getNum(), rdb->getType()}, rdb->getKey()}, rdb}); 
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
            res->set_reply("command is not find!") ;
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

string redisDb :: findHgetRequest(const string key, 
                                  const string feild) {
    auto res = db.find({{num, DB_HASH}, key}) ;
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
        cout << num <<  s.first << s.second <<"    " <<  type<< endl ;
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
string redisDb :: findGetRequest(const string key, const int num) {
    //查找
    auto res= db.find({{num, DB_STRING}, key}) ;
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

//字符串的形式传进来
void hashSet :: setValue(string value, ...) {
    string key = value ;
    va_list va ;
    va_start(va, value) ;
    char* val = va_arg(va, char*) ;
    values[key] = val ;
    va_end(va) ;
}
