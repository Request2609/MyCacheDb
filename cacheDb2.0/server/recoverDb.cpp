#include "recoverDb.h"

shared_ptr<redisDb> recoverDb :: recover(string& s, cmdSet* cmdset) {
    if(!isRedis(s)) {
        return nullptr ;
    }
    int flag = 0 ;
    int num = getNum(s) ;
    shared_ptr<redisDb> db = cmdset->getDB(num) ;
    if(db == nullptr) {
        flag = 1 ;
        db = make_shared<redisDb>(num) ;
    }
    //记录之前的对象的种类
    int preType =  -1 ;
    while(1) {
        long times = getTime(s) ;
        if(times == 0) {
            break ;
        }
        //超时的话，就找下一个
        if(times > 0 && times < timer :: getCurTime()) {
            long index = s.find("\r$\n") ;
            s = s.c_str() + index + 3;
            continue ;
        }
        int  type = getType(s) ;
        if(type == CMDTYPE_::STRING) {
            shared_ptr<dbObject>ob = factory :: getObject("set") ;
            stringGet(s, ob) ;
            ob->setEndTime(times) ;
            cmdset->addObjectToDb(num, ob) ;
        }
        if(type == CMDTYPE_::DB_HASH) {
            shared_ptr<dbObject> ob = factory :: getObject("hset") ;
            ob->setType(DB_HASH) ;
            ob->setNum(num) ;
            ob->setEndTime(times) ;
            int ret = hashGet(s, ob) ;  
            if(ret < 0) {
                return nullptr ;
            }
            cmdset->addObjectToDb(num, ob) ;
        }
    }
    if(flag == 1) {
        cmdset->append(db) ;
    }
    return db ;
}

int recoverDb :: hashGet(string& s, shared_ptr<dbObject>&ob) {
    string key = getHashKey(s) ;
    ob->setKey(key) ;
    while(1) {
        pair<string, string> pp = getAttrKV(s) ;
        if(pp.first == "" || pp.second == "") {
            break ;
        }
        ob->setValue(pp.first, pp.second.c_str()) ;
    }
}

bool recoverDb :: isOk(long index) {
    if(index == (long)string :: npos) {
        return 0 ;
    }
    return 1 ;
}

pair<string, string> recoverDb :: getAttrKV(string& s) {
    long index = s.find(":") ;
    if(!isOk(index)) {
            return {"", ""} ;
    }
    string key = s.substr(0, index) ;
    s = s.c_str() + index + 1 ;
    index = s.find("\r\n") ;
    if(!isOk(index)) {
        return {"", ""} ;
    }
    string value = s.substr(0, index) ;
    s = s.c_str() + index + 2 +index ; 
    pair<string, string> pp = make_pair(key, value) ;
    return pp ;
}

//获取key对象
string recoverDb :: getHashKey(string& s) {
    int index = s.find("\r\n") ;
    string key ;
    long i = 0 ;
    for(i=index+2; s[i] != '\r'; i++) {
        key += s[i] ;
    }
    s = s.c_str()+2 ;
    return key ;
}

int recoverDb :: getXC(string & s) {
    long index = s.find("xc:") ;
    string xc ;
    for(long i=index+3; s[i] != '\r'; i++) {
        xc += s[i] ;
    }
    s = s.c_str()+index+3 ;
    return atoi(xc.c_str()) ;
}   

void recoverDb :: stringGet(string& s, shared_ptr<dbObject>&ob) {
    string value ;
    string key ;
    int ecode = getEncoding(s) ;
    if(ecode == STRING_::REDIS_ENCODING_INT) {
        key = getKey(s, value) ;
    }
    else if(ecode == STRING_ ::REDIS_ENCODING_RAW){
        long ss = getYC(s) ;
        int dn = 0 ;
        ///原长大于20字节,获取压缩后的长度
        if(ss > 20) {
            dn = getXC(s) ;
        }
        key = getKey(s) ;
        value = getValue(s);
        //解码
        if(ss > 20){   
            char* val = (char*)malloc(ss) ;
            int ret = lzf_decompress(value.data(), dn, val, ss) ;
            if(ret == 0) {
                free(val) ;
                cout << __FILE__ << "   "<<__LINE__ << endl ;
                return ;
            }
            value = val ;
            free(val) ;
        }
    }
    ob->setType(DB_STRING) ; 
    ob->setKey(key) ;
    ob->setValue(value) ;
}

//获取键值
string recoverDb :: getKey(string& s) {
    long index = s.find("\r\n") ;
    string key ;
    long i = 0 ;
    for(i=index+2; s[i] !=':'; i++) {
        key += s[i] ; 
    }
    s = s.c_str()+index+2 ;
    return key ;
}

long recoverDb :: getYC(string& s) {
    long index = s.find("yc:") ;
    long i = 0 ;
    string yc ;
    for(i=index+3; s[i]!='\r'; i++) {
        yc+=s[i] ;
    }
    s = s.c_str() + index + 3 ;
    return atol(yc.c_str()) ;
}


string recoverDb :: getKey(string& buf, string& value) {
    long index = buf.find("\r\n") ;
    string key ;
    long i, j ;
    for(i=index+2; buf[i] != ':'; i++) {
        key += buf[i] ;
    }
    for(j= i+1; buf[j] != '\r'; j++) {
        value += buf[j] ;
    }
    buf = buf.c_str() + j;
    return key ;
}

string recoverDb :: getValue(string& s) {
    long index = s.find(":") ;
    string value ;
    long i = 0 ;
    for(i=index+1; s[i] != '\r'; i++) {
        value += s[i] ;
    }
    s = s.c_str()+index ;
    return value ;
}

int recoverDb :: getEncoding(string& buf) {
    long index = buf.find("ec") ;
    string code ;
    for(long i=index+3; buf[i] != '\r'; i++) {
        code +=  buf[i] ;
    }
    buf = buf.c_str() + index + 3;
    return atoi(code.c_str()) ;

}
int recoverDb :: getType(string& s) {
    long  index = s.find("ctp:") ;
    assert(index != string ::npos) ;
    string ss ;
    for(long i=index+4; s[i]!= '\r'; i++) {
        ss += s[i] ;
    }
    s = s.c_str()+index+4 ;
    return atoi(ss.c_str()) ;
}

bool recoverDb :: isRedis(string& buffer) {

    string name ;
    for(long i=0; i<5; i++) {
        name+=buffer[i] ;
    }   
    if(name == "redis") {
        buffer = buffer.c_str()+5 ;
        return true ;
    }
    return false ;
}

//获取过期时间
long recoverDb :: getTime(string& buf) {
    long  index = buf.find("e:") ;
    cout << index << endl ;
    //没找到
    if(index == (long)string :: npos) {
        return 0 ;
    }
    string end ;
    for(long i=index+2; buf[i] != '\r'; i++) {
        end+=buf[i] ;
    }
    buf = buf.c_str() + index+2 ;
    return atoll(end.c_str()) ;
}

int recoverDb :: getNum(string& buf) {
    long index = buf.find("id:") ;
    string n ;
    for(long i=index+3; buf[i] != '\r'; i++) {
        n+=buf[i] ;
    }
    //将解析过的字符丢弃
    buf = buf.c_str() + index ;
    return atoi(n.c_str()) ;
}
