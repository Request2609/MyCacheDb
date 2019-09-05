#include "redisDb.h"

//判断当前操作键值存在不存在
int redisDb :: isExist(shared_ptr<Command>&cmds) {
    string cmd = cmds->cmd() ;
    //如果是set 命令
    if(cmd == "set") {
        string key = cmds->keys(0).key(0) ;
        //查找相应的键值
        vector<shared_ptr<dbObject>>::iterator iter ;
        for(iter = db.begin(); iter != db.end(); iter++) {
            if(key == (*iter)->getKey()) {
               //修改键对应的值
                (*iter)->setValue(cmds->vals(0).val(0)) ;
                return 1 ;
            }
        }
        return 0 ;
    }
}

//查询数据库,get命令等
void redisDb :: queryDb(shared_ptr<Response>& res, shared_ptr<Command>& cmd) {

    //get查询
    string md = cmd->cmd() ;
    if(!strcasecmp(md.c_str(), "get")) {
        //在本数据库中找set对象并且查询的值
        string key ;
        //获取到键值
        key = cmd->keys(0).key(0) ;
        string r = findGetRequest(key) ;
        if(!r.size()) {
            res->set_reply("command is not find!") ;
        }
        else {
            res->set_reply("\""+r+"\"") ;
        }
    }
}

//找相应的get请求键的值
string redisDb :: findGetRequest(string key) {

    //查找
    for(shared_ptr<dbObject>rdb : db) {
        if(rdb->getName() == "set" && rdb->getKey() == key) {
            return rdb->getValue() ;
        }
    }
    return "" ;
}

