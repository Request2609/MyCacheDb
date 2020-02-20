#include "recoverDb.h"


int recoverDb :: getTime() {
    time_t t ;
    t = time(NULL) ;
    int ii = time(&t) ;
    return ii ;
}

std::shared_ptr<redisDb> recoverDb :: recover(string& s, cmdSet* cmdset) {
    if(!isRedis(s)) {
        return nullptr ;
    }
    int flag = 0 ;
    int num = getNum(s) ;
    std::shared_ptr<redisDb> db = cmdset->getDB(num) ;
    if(db == nullptr) {
        flag = 1 ;
        db = make_shared<redisDb>(num) ;
    }
    //记录之前的对象的种类
    while(1) {
        long times = getTime(s) ;
        if(times == 0) {
            break ;
        }
        //超时的话，就找下一个
        if(times > 0 && times < getTime()) {
            //每个对象元素结束的标识
            long index = s.find("\r$\n") ;
            s = s.c_str() + index + 3;
            continue ;
        }

        int  type = getType(s) ;
        if(type == CMDTYPE_::STRING) {
            std::shared_ptr<dbObject>ob = factory :: getObject("set") ;
            stringGet(s, ob) ;
            ob->setEndTime(times) ;
            cmdset->addObjectToDb(num, ob) ;
        }
        if(type == CMDTYPE_::DB_HASH) {
            std::shared_ptr<dbObject> ob = factory :: getObject("hset") ;
            ob->setType(type::DB_HASH) ;
            ob->setNum(num) ;
            ob->setEndTime(times) ;
            int ret = hashGet(s, ob) ; 
            if(ret < 0) {
                return nullptr ;
            }
           cmdset->addObjectToDb(num, ob) ;
        }
        
        if(type == CMDTYPE_::DB_LIST) {
            std::shared_ptr<dbObject> ob = factory :: getObject("lpush") ;
            ob->setType(CMDTYPE_::DB_LIST) ;
            ob->setNum(num) ;
            ob->setEndTime(times) ;
            getListObject(s, ob) ;
            cmdset->addObjectToDb(num, ob) ;
        }
        
    }
    if(flag == 1) {
        cmdset->append(db) ;
    }
    
    return db ;
}


std::string recoverDb:: getListValues(std::string& s) {
    std::string ss="" ;
    int i = 0 ;
    for(i=0; s[i]!='\r' && s[i]!='\n'; i++) {
        if(s[i] == '$') {
            return ss ;
        }
        ss += s[i] ;    
    }
    s = s.substr(i+2) ; 
    return ss ;
}

int recoverDb :: getListObject(std::string& s, std::shared_ptr<dbObject>& ob) {
    //获取键值
    std::string key = getHashKey(s) ;    
    ob->setKey(key) ;
    //找值
    while(1) {
        std::string ss = getListValues(s) ;
        if(ss == "") break ;
        ob->setValue(ss.c_str()) ;
    }
    return 1 ;
}

int recoverDb :: hashGet(std::string& s, std::shared_ptr<dbObject>&ob) {
    std::string key = getHashKey(s) ;
    ob->setKey(key) ;
    while(1) {
        std::pair<std::string, std::string> pp = getAttrKV(s) ;
        if(pp.first == "" || pp.second == "") {
            break ;
        }
        ob->setValue(pp.first, pp.second.c_str()) ;
    } 
    return 1 ;
}

bool recoverDb :: isOk(long index) {
    if(index == (long)std::string :: npos) {
        return 0 ;
    }
    return 1 ;
}

std::pair<std::string, std::string> recoverDb :: getAttrKV(std::string& s) {
    long index = s.find(":") ;
    int end = s.find("\r$\n") ;
    if(end == 0) {
        return {"", ""} ;
    }
    if(!isOk(index)) {
        return {"", ""} ;
    }
    std::string key = s.substr(0, index) ;
    s = s.c_str() + index + 1 ;
    index = s.find("\r\n") ;
    if(!isOk(index)) {
        return {"", ""} ;
    }
    std::string value = s.substr(0, index) ;
    s = s.c_str() + index + 2 ; 
    std::pair<std::string, std::string> pp = make_pair(key, value) ;
    return pp ;
}

//获取key对象
std::string recoverDb :: getHashKey(std::string& s) {
    //找'\r\n'
    long index = s.find("\r\n") ;
    std::string key ;
    long i = 0 ;
    //从'\r\n'后面开始找键，碰到'r'结束
    for(i=index+2; s[i] != '\r'; i++) {
        key += s[i] ;
    }
    s = s.c_str()+2+i ;
    return key ;
}

int recoverDb :: getXC(std::string & s) {
    long index = s.find("xc:") ;
    std::string xc ;
    for(long i=index+3; s[i] != '\r'; i++) {
        xc += s[i] ;
    }
    s = s.c_str()+index+3 ;
    return atoi(xc.c_str()) ;
}   

void recoverDb :: stringGet(std::string& s, std::shared_ptr<dbObject>&ob) {
    std::string value ;
    std::string key ;
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
            //原长不等于从文件中读出的现长，压缩过
            //否则在压缩的时候失败了，不用解压缩
            if(dn != ss) {
                char* val = (char*)malloc(ss) ;
                int ret = lzf_decompress(value.data(), dn, val, ss) ;
                if(ret == 0) {
                    free(val) ;
                }
                else {
                    value = val ;
                    free(val) ;
                }
            }
        }
    }
    ob->setType(type::DB_STRING) ; 
    ob->setKey(key) ;
    ob->setValue(value) ;
}

void recoverDb::recoverByLog(std::vector<std::pair<int, std::shared_ptr<redisDb>>>*dbLs) {
    struct stat st ;
    //查看备份文件
    int fd = open("../logInfo/backuplog.log", O_RDONLY) ;
    if(fd > 0) {
        fstat(fd, &st) ;
        char* buf = (char*)mmap(buf, st.st_size, PROT_READ, fd, MAP_SHARED,0) ;
        parseAndExecAofCmd(buf, *dbLs) ;
    }
    fd = open("../logInfo/aoflog.log", O_RDONLY) ;
    if(fd > 0) {
        fstat(fd, &st) ;
        char* buf = (char*)mmap(buf, st.st_size, PROT_READ, fd, MAP_SHARED,0) ;
        parseAndExecAofCmd(buf, *dbLs) ;
    }
}

std::shared_ptr<redisDb>recoverDb::getDb(int num, const std::vector<std::pair<int, std::shared_ptr<redisDb>>>&ls) {
    for(auto s=ls.begin(); s!=ls.end(); s++) {
        if(s->first == num) {
            return s->second ;
        }
    }
    return nullptr ;
}

void recoverDb :: parseAndExecAofCmd(const char* buf, std::vector<std::pair<int, std::shared_ptr<redisDb>>>&dbls) {
    std::string s = buf ;
    std::string tmp ;
    int index = 0 ;
    std::shared_ptr<Command>cmd = make_shared<Command>();
    std::shared_ptr<redisDb>wcmd ; 
    std::shared_ptr<Response>rs ;
    std::vector<std::string>ls ;
    while(s.size() != 0) {
        if(s.find('\n') == std::string::npos) {
            break ;
        }
        else {
            int num = 0 ;
            index = s.find('\n') ;
            tmp=s.substr(0, index) ;
            ls = split(tmp," ") ;
            if(ls.size() > 2) {
                if(ls[2] == "set") {
                     num = aofString(cmd, ls) ;
                     wcmd = getDb(num, dbls) ;
                     cmdCb::setCmd(wcmd, cmd, rs) ;
                }
                else if(ls[2] == "hset") {
                    num = aofHash(cmd, ls) ;
                    wcmd = getDb(num, dbls) ;
                    cmdCb::setHash(wcmd, cmd, rs) ;
                }
                else if(ls[2] == "sadd") {
                    wcmd = getDb(num, dbls) ;
                    num =aofSet(cmd, ls) ;
                    cmdCb::setSetValue(wcmd, cmd, rs) ;
                } 
                else if(ls[2] == "zadd"){
                    num = aofSortSet(cmd, ls) ;
                    wcmd = getDb(num, dbls) ;
                    cmdCb::sortSetAdd(wcmd, cmd, rs) ;
                }
                else if(ls[2] == "lpush") {
                    num = aofList(cmd, ls) ;
                    wcmd = getDb(num, dbls) ;
                    cmdCb::setLpush(wcmd, cmd, rs) ;
                }
            }
            else {
                break ;
            }
            s = s.c_str()+index+1 ;
        }
    }
}

int recoverDb::aofString(std::shared_ptr<Command>&cmd, const std::vector<std::string>& str) {
    int num = atoi(str[1].c_str()) ;
    cmd->set_num(num) ;
    cmd->set_cmd(str[2]) ;   
    cmd->set_type(type::DB_STRING);
    Key* key = cmd->add_keys() ;
    std::string* k = key->add_key() ;
    *k = str[3] ;
    Value* val = cmd->add_vals() ;
    std::string* v = val->add_val() ;
    //设置数据库编号
    *v = str[4] ;
    return num ;
}


int recoverDb::aofHash(std::shared_ptr<Command>&cmd, const std::vector<std::string>& str) {
    int num = atoi(str[1].c_str()) ;
    cmd->set_num(num) ;
    cmd->set_type(type::DB_HASH) ;
    int len = str.size() ;
    cmd->set_cmd(str[2]) ;
    Key* key = cmd->add_keys() ;
    std::string* k = key->add_key() ;
    *k = str[3] ;
    Value* val = cmd->add_vals() ;
    Key* keyk = cmd->add_keys() ;
    for(int i=3; i<len; i+=2) {
        std::string* v = val->add_val() ;
        *v = str[i+1] ;
        std::string* k = keyk->add_key() ;
        *k = str[i] ;
    }
    return num ;
}

int recoverDb::aofList(std::shared_ptr<Command>&cmd, const std::vector<std::string>& str) {
    int num = atoi(str[1].c_str()) ;
    cmd->set_num(num) ;
    cmd->set_type(type::DB_LIST) ;
    ListObject *lob = cmd->add_lob() ;
    cmd->set_cmd(str[2]) ;
    lob->set_key(str[3]) ;
    int len = str.size() ;
    Value* val = lob->add_vals() ;
    for(int i=4; i<len; i++) {
        std::string*v = val->add_val() ;
        *v = str[i] ;
    }
    cmd->set_num(atoi(str[1].c_str()));
    return num ;
}

int recoverDb::aofSet(std::shared_ptr<Command>&cmd, const std::vector<std::string>& str) {
    int num = atoi(str[1].c_str()) ;
    cmd->set_type(type::SET_SET) ;
    cmd->set_num(num);
    ListObject *lob = cmd->add_lob() ;
    cmd->set_cmd(str[2]) ;
    lob->set_key(str[3]) ;
    int len = str.size() ;
    Value* val = lob->add_vals() ;
    for(int i=4; i<len; i++) {
        std::string*v = val->add_val() ;
        *v = str[i] ;
    }
    return num ;
}

int recoverDb::aofSortSet(std::shared_ptr<Command>&cmd, const std::vector<std::string>& str) {
    int num = atoi(str[1].c_str()) ;
    cmd->set_num(num) ;
    cmd->set_type(type::SET_SET) ;
    cmd->set_cmd(str[2]) ;
    ListObject* lob = cmd->add_lob() ;
    lob->set_key(str[3]) ;
    Value* val = lob->add_vals() ;
    std::string*s = val->add_val() ;
    *s = str[4] ;
    std::string*s1 = val->add_val() ;
    *s1 = str[5] ;
    return num ;
}
//使用gpb命名空间
std::vector<std::string> recoverDb::split(const std::string &s, const std::string &seperator){
    std::vector<std::string> result;
    typedef std::string::size_type string_size;
    string_size i = 0;
    while(i != s.size()){
        //找到字符串中首个不等于分隔符的字母；
        int flag = 0;
        while(i != s.size() && flag == 0){ 
            flag = 1;
            for(string_size x = 0; x < seperator.size(); ++x)
                if(s[i] == seperator[x]){
                    ++i;
                    flag = 0;
                    break;
                }
        }
        //找到又一个分隔符，将两个分隔符之间的字符串取出；
        flag = 0;
        string_size j = i;
        while(j != s.size() && flag == 0){ 
            for(string_size x = 0; x < seperator.size(); ++x)
                if(s[j] == seperator[x]){
                    flag = 1;
                    break;
                }
            if(flag == 0)                                                                                                                                                                                                                                                     
                ++j;
        }
        if(i != j){ 
            result.push_back(s.substr(i, j-i));
            i = j;
        }
    }   
    return result;
}      
//获取键值
std::string recoverDb :: getKey(std::string& s) {
    long index = s.find("\r\n") ;
    std::string key ;
    long i = 0 ;
    for(i=index+2; s[i] !=':'; i++) {
        key += s[i] ; 
    }
    s = s.c_str()+index+2 ;
    return key ;
}

long recoverDb :: getYC(std::string& s) {
    long index = s.find("yc:") ;
    long i = 0 ;
    std::string yc ;
    for(i=index+3; s[i]!='\r'; i++) {
        yc+=s[i] ;
    }
    s = s.c_str() + index + 3 ;
    return atol(yc.c_str()) ;
}


std::string recoverDb :: getKey(std::string& buf, std::string& value) {
    long index = buf.find("\r\n") ;
    std::string key ;
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

std::string recoverDb :: getValue(std::string& s) {
    long index = s.find(":") ;
    std::string value ;
    long i = 0 ;
    for(i=index+1; s[i] != '\r'; i++) {
        value += s[i] ;
    }
    s = s.c_str()+index ;
    return value ;
}

int recoverDb :: getEncoding(std::string& buf) {
    long index = buf.find("ec") ;
    std::string code ;
    for(long i=index+3; buf[i] != '\r'; i++) {
        code +=  buf[i] ;
    }
    buf = buf.c_str() + index + 3;
    return atoi(code.c_str()) ;

}
int recoverDb :: getType(std::string& s) {
    long  index = s.find("ctp:") ;
    assert(index != std::string ::npos) ;
    std::string ss ;
    for(long i=index+4; s[i]!= '\r'; i++) {
        ss += s[i] ;
    }
    s = s.c_str()+index+4 ;
    return atoi(ss.c_str()) ;
}

bool recoverDb :: isRedis(std::string& buffer) {

    std::string name ;
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
long recoverDb :: getTime(std::string& buf) {
    long  index = buf.find("e:") ;
    //没找到
    if(index == (long)std::string :: npos) {
        return 0 ;
    }
    std::string end ;
    for(long i=index+2; buf[i] != '\r'; i++) {
        end+=buf[i] ;
    }
    buf = buf.c_str() + index+2 ;
    return atoll(end.c_str()) ;
}

int recoverDb :: getNum(std::string& buf) {
    long index = buf.find("id:") ;
    std::string n ;
    for(long i=index+3; buf[i] != '\r'; i++) {
        n+=buf[i] ;
    }
    //将解析过的字符丢弃
    buf = buf.c_str() + index ;
    return atoi(n.c_str()) ;
}

