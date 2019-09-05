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

