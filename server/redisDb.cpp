#include "redisDb.h"

//判断当前操作键值存在不存在,找到并修改
int redisDb :: isExist(std::shared_ptr<Command>&cmds) {
    auto ptr = aofRecord::getLogObject() ;
    std::string s = "" ; 
    std::string cmd = cmds->cmd() ;
    //如果是set 命令
    int num = cmds->num() ;
    if(cmd == "set") {
        s=to_string(cmds->num())+" set" ;
        std::string k = cmds->keys(0).key(0) ;
        key ke(num, type::DB_STRING, k) ;

        auto res = db.find(ke) ;
        if(res == db.end()) {
            return 0 ;
        }
        res->second->timer = curTimer::curTime() ;
        s+=" "+k+" "+cmds->vals(0).val(0) ;
        res->second->setNum(cmds->num()) ;
        res->second->setValue(cmds->vals(0).val(0)) ;
        ptr->record(s.c_str()) ;
        return 1 ;
    }
    if(cmd == "hset") {
        s += std::to_string(num)+" "+"hset" ;
        std::string k = cmds->keys(0).key(0) ;
        std::string kk = cmds->keys(1).key(0) ;
        std::string vv = cmds->vals(0).val(0) ;
        s+=" "+k+" "+kk+" "+vv ;
        key ke(num, type::DB_HASH, k) ;
        auto res = db.find(ke) ;
        if(res == db.end()) {
            return 0 ;
        }
        res->second->timer = curTimer::curTime() ;
        int k_len = cmds->keys_size() ;
        for(int i=1; i<k_len; i++) {
            int lk = cmds->keys(i).key_size() ;
            int lv = cmds->vals(i-1).val_size() ;
            if(lk != lv) {
                return -1 ;
            }
            for(int j=0; j<lk; j++) {
                std::string kk = cmds->keys(i).key(j) ;
                std::string vv = cmds->vals(i-1).val(j) ;
                res->second->setValue(kk, vv.c_str()) ;
                s+=" "+kk+" "+vv ;
            }
        }
        res->second->setNum(cmds->num()) ;
        ptr->record(s.c_str()) ;
        return 1 ;
    }
    ListObject lob = cmds->lob(0) ;
    if(cmd == "lpush") {
        s+=" "+std::to_string(num)+" "+"lpush" ;
        std::string keys = lob.key() ;
        s+=" "+keys ;
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
                s+=" "+lob.vals(i).val(j) ;
            }
        }
        res->second->timer = curTimer::curTime() ;
        num = res->second->objectSize() ;
        ptr->record(s.c_str()) ;
        return num ;   
    }
    if(cmd == "zadd") {
        std::string keys = lob.key() ;
        s+=" "+std::to_string(num)+" "+"zadd"+" "+keys ;
        key ke(num, type::SORT_SET, keys) ;
        auto res = db.find(ke) ;
        if(res == db.end()) {
            return -1 ;
        }
        res->second->timer = curTimer::curTime() ;
        Value val = lob.vals(0) ;
        std::string score = val.val(0);
        std::string value = val.val(1) ;
        s+=" "+score+" "+value ;
        res->second->setValue(score, value.c_str()) ;
        ptr->record(s.c_str()) ;
    }

    if(cmd == "sadd") {
        std::string keys = lob.key() ;
        s+=" "+std::to_string(num)+" "+keys ;
        key ke(num, type::SET_SET, keys) ;
        auto res = db.find(ke) ;
        if(res == db.end()) {
            return -1 ;
        }
        res->second->timer = curTimer::curTime() ;
        Value val = lob.vals(0) ;
        int size = val.val_size() ;
        for(int i=0; i<size; i++) {
            res->second->setValue(val.val(i)) ;
            s+=" "+val.val(i) ;
        }
        ptr->record(s.c_str()) ;
    }
    return 1 ;
}

long curTimer::curTime() {
    time_t t ;
    t = time(NULL) ;
    long ii = time(&t) ;
    return ii ;
}

int redisDb::append(int num, int type, std::shared_ptr<dbObject>dob) {
    key k ;
    k.num = num ;
    k.type = type ;
    k.cmd = dob->getKey() ;
    k.timer = curTimer::curTime() ;
    auto s = db.find(k) ;
    if(s == db.end()) {
        db.insert({k, dob}) ;
        auto ret = db.find(k) ;
        ret->second->timer = curTimer::curTime() ;
    }
    //判断内存上限
    if(isFull(simpleLru::getMem())) {
        key k = getRandomKey() ;
        k.timer = db[k]->timer;
        simpleLru::statistic(k) ;
    }
}

//产生随机数
key redisDb::getRandomKey() {
    int num = db.bucket_count() ;
    key k ;
    while(1) {
        int index = rand()%num ;
        int flag = 0 ;
        for(auto it=db.begin(index); it!=db.end(index); it++) {
            flag = 1 ;
            key k = it->first ;
            break ;
        }
        if(flag == 1) break ;
    }
}
    
//遍历redis中的dbObject对象
std::shared_ptr<dbObject> redisDb :: getNextDb() {
    static auto res = db.begin();
    if(res == db.end() || !db.size()) { 
        res = db.begin() ;
        return nullptr ;
    }
    auto re = res ;
    std::shared_ptr<dbObject>dob = re->second ;
    res ++ ;
    int flag = 0 ;
    //判断当前超时
    long time = recoverDb :: getTime() ;
    int end = dob->getEndTime() ;
    //超时值大于０的时候，判断超时,小于０表示是永久保存
    if(end > 0 && time > end) {
        flag = 1 ;
        db.erase(re) ;
    }
    return flag == 0 ? dob:nullptr ;
}

void redisDb :: append(std::shared_ptr<dbObject>rdb) {   
    //三元组确定对象
    key k  ;
    //数据库号
    k.num = rdb->getNum() ;
    //类型
    k.type = rdb->getType() ;
    //键值
    k.cmd = rdb->getKey() ;
    k.timer = curTimer::curTime() ;
    rdb->timer = curTimer::curTime() ;
    db.insert({k, rdb}); 
    if(isFull(simpleLru::getMem())) {
        key k = getRandomKey() ;
        k.timer = db[k]->timer;
        simpleLru::statistic(k);
    }
}

//查询数据库,get命令等
int redisDb :: queryDb(std::shared_ptr<Response>& res, std::shared_ptr<Command>& cmd) {
    //get查询
    std::string md = cmd->cmd() ;
    int num = cmd->num() ;
    if(!strcasecmp(md.c_str(), "get")) {
        //在本数据库中找set对象并且查询的值
        std::string key ;
        //获取到键值
        key = cmd->keys(0).key(0) ;
        std::string r = findGetRequest(key, num) ;
        if(!r.size()) {
            res->set_reply("object is not find!") ;
        }
        else {
            res->set_reply("\""+r+"\"") ;
        }
    }

    if(!strcasecmp(md.c_str(), "hget")) {
        std::string value ;
        std::string key = cmd->keys(0).key(0) ;
        value = cmd->keys(0).key(1) ;
        std::string feild = findHgetRequest(key, value) ;
        if(feild == "") {
            res->set_reply("no the object!") ;
            return 1;
        }
        res->set_reply("\""+feild+"\"") ;
    }
    int flag = 0;
    if(!strcasecmp(md.c_str(), "lpop") || !strcasecmp(md.c_str(), "blpop")) {
        std::string key = cmd->lob(0).key() ;
        std::string val = findListRequest(key, num) ;
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
        std::string key = cmd->lob(0).key() ;
        std::string val = findSortSetValue(cmd) ;
        if(val.empty()) {
           return -1 ; 
        }
        res->set_reply(val) ;
    }
    if(!strcasecmp(md.c_str(), "spop")) {
        std::string key = cmd->lob(0).key() ;
        std::string val = findSetRequest(key, num) ;
        if(val.empty()) {
            return -1 ;
        }
        res->set_reply(val) ;
    }
    return 1 ;
}

std::string redisDb :: findSetRequest(const std::string k, const int num ){
    key ke ;
    ke.cmd = k ;
    ke.num = num ;
    ke.type= type::SET_SET ;
    auto res = db.find(ke) ;
    if(res == db.end()) {
        return "" ;
    }
    std::string ss = res->second->getValue() ;
    res->second->timer = curTimer::curTime() ;
    return ss ;  
}

std::string redisDb :: findSortSetValue(const std::shared_ptr<Command>& cmd) {
    key  ke ;
    ListObject lob = cmd->lob(0) ;
    ke.cmd = lob.key() ;
    std::string s = lob.vals(0).val(0) ;
    std::string e = lob.vals(0).val(1) ;
    std::string val = s+" "+e ;
    ke.num = cmd->num() ;
    ke.type = type::SORT_SET ;
    auto ret = db.find(ke) ;
    if(ret == db.end()) {
        return "" ;
    }
    vector<std::string> res = ret->second->getValues(val) ;
    ret->second->timer = curTimer::curTime() ;
    //将结果集合打包
    std::string ss = "" ;
    for(auto a : res) {
        ss+=a ;
        ss+='\n' ;
    }
    ss[ss.size()-1] = ' ';
    return ss ;
}

void redisDb :: processBlpop() {
    
}


std::string redisDb::findListRequest(const std::string k, const int num) {
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
    res->second->timer = curTimer::curTime() ;
    std::string t= res->second->getValue() ;
    int len = res->second->objectSize() ;   
    return t+" "+to_std::string(len) ;
}

void redisDb :: removeDataByKey(key k) {
    for(auto s=db.begin(); s != db.end(); s++) {
        if(k == s->first){
            db.erase(s) ;
            break ;
        }
    }
}


std::string redisDb :: findHgetRequest(const std::string k, 
                                  const std::string feild) {
    key ke ;
    ke.num = num ;
    ke.type = type::DB_HASH ;
    ke.cmd = k ;
    auto res = db.find(ke) ;
    if(res == db.end()) {
        return "no the object!" ;
    }
    //获取过期时间
    long endTime = res->second->getEndTime() ;
    if(endTime != -1 && endTime < recoverDb::getTime()) {
        db.erase(res) ;
        return "" ;
    }
    res->second->timer = curTimer::curTime() ;
    auto s = res->second->getValues(feild) ;
    return s[0] ;   
}
void redisDb :: print() {
    for(auto res=db.begin(); res != db.end(); res++) {
        res->second->print() ;
    }
}
//找相应的get请求键的值
std::string redisDb :: findGetRequest(const std::string k, const int num) {
    //查找
    key ke(num, type::DB_STRING, k) ;
    auto res= db.find(ke) ;
    if(res == db.end()) {
        return "" ;
    }
    else {
        res->second->timer = curTimer::curTime() ;
        long ti = res->second->getEndTime() ;
        if(ti != -1 && ti < recoverDb::getTime()) {
            return "" ;
        } 
        return res->second->getValue() ; 
    }
}


