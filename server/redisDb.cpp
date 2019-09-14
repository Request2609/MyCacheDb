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

/*
void dbObject :: addString(string key, string value) {
    str.insert({key, value}) ;
}

string dbObject :: getString(string key) {  
    map<string, string>::iterator iter = str.find(key) ;
    if(iter == str.end()) {
        return "" ;
    }
    else {
        return str[key] ;
    }
}*/

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
}

void setCommand :: print() {
    cout << type<< "    " << num<< "   "<< key << "    "<< value << endl ;
}

void redisDb :: print() {
    for(auto res=db.begin(); res != db.end(); res++) {
        cout <<"数据库编号"<< res->second->getNum() << endl ;
           cout <<"键值："<<"    " << res->second->getKey() << ":" 
            << res->second->getValue() << "类型："<< res->second->getType()<<endl ;
    }
}
//找相应的get请求键的值
string redisDb :: findGetRequest(string key, int num) {
    //查找
    auto res= db.find({{num, DB_STRING}, key}) ;
    if(res == db.end()) {
        cout << "没招到" << endl ;
        return "" ;
    }
    else {
        return res->second->getValue() ;
    }
}

