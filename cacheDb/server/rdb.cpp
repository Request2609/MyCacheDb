#include "rdb.h"

//保存到文件
int rdb :: save(const shared_ptr<redisDb> db, char* fileName) {  
    
    ofstream out(fileName, ios::out|ios::binary|ios::trunc) ;
    //以二进制写形式创建rdb文件
    if(out.fail()) {
       cout << __FILE__ << "    " << __LINE__ << endl ;
        return -1;
    }
    int num = db->getId() ;
    //数据库编号
    string head = makeHeader() ;
    //将头部写入文件
    out << head ;
    out << "id:" << num <<"\r\n" ;
    //获取数据库中的对象元素
    shared_ptr<dbObject> rd = db->getNextDb() ;
    while(rd != nullptr) {
        string value = rd->getValue() ;
        //字符串类别是string
        if(rd->getType() == DB_STRING) {
            string key = rd->getKey() ;
            //设置数据库编号
            //将键的长度和键值写入文件
            //-1设置过期时间为永久
            out << "e:" ;
            //超时时间
            out << rd->getEndTime() << "\r\n" ;
            //命令类型
            out << "ctp:" << CMDTYPE::STRING << "\r\n" ;
            //值要根据不同的情况进行编码
            processString(key, out, value) ;
        }
        rd = db->getNextDb() ;
    }
    out.close() ;
    return 1 ;
}

void rdb :: processString(const string key, ofstream& out, const string value) {

    const char * v = value.c_str() ;
    //编码类型
    int type = getStringEncodingType(value) ;
    //值是以字符串，使用raw编码
    //分为两种编码形式，一种是小于20字节的原样保存
    //一种是大于20字节的压缩保存
    if(type == ENCODING_INT::INT8) {
        //编码形式
        out << "ec:" << ENCODING_INT::INT8 << "\r\n" ;
        //值
        int8_t a = atoi(v) ;
        //将键值写入到文件中
        out << key <<":" << a << "\r\n" ;
    }
    else if(type == ENCODING_INT::INT16) {
        out << "ec:" << ENCODING_INT::INT8 << "\r\n" ;
        //值
        int16_t a = atoi(v) ;
        //将键值写入到文件中
        out << key <<":" << a << "\r\n" ;
        
    }
    else if(type == ENCODING_INT::INT32) {
        out << "ec:" << ENCODING_INT::INT32 << "\r\n" ;
        //值
        int32_t a = atoi(v) ;
        //将键值写入到文件中
        out << key <<":" << a << "\r\n" ;
    }
    else if(type == STRING::REDIS_ENCODING_RAW) {
        out << "ec:" << STRING::REDIS_ENCODING_RAW << "\r\n" ;
        //值
        int size = value.size() ;
        if(size <= 20) {
            out << STRING_ZIP :: NO_CHANGE<<"\r\n" ;
            out << key <<":" << value;                 
        }
        else {
            out << key << ":" ;
            long len = 0 ;
            len = value.size() ;
            out << "yc:" << len << "\r\n" ;
            //标志是否压缩过
            out << STRING_ZIP :: ENCODING_LZF <<"\r\n" ;
            //压缩值
            //将字符串原长保存到文件
            void* ot = lzfCompress(value) ;
            const char* ch = (char*)ot ;
            out << ch <<"\r\n\r\n" ;
            //销毁ot内存
            free(ot) ;
        }
    }
    return  ;
}

//压缩
void* rdb :: lzfCompress(const string& value) {
    size_t len = value.size() ;
    size_t outlen = len-4 ;
    void* out ;
    if((out = malloc(outlen+1)) == NULL) {
        cout << __FILE__ << "     " << __LINE__ << endl ;
        return NULL ;
    }
    size_t l = lzf_compress(value.data(), len, out, outlen) ;
    if(l == 0) {
        free(out) ;
        return NULL ;
    }
    return out ;
}   

int rdb :: getStringEncodingType(const string value) {

    if(isNum(value.c_str())) {
        //如果int小于32为整数
        if(atoll(value.c_str()) < INT32_MAX) {
            int a = atoi(value.c_str()) ;
            if(a < INT8_MAX) {
                return ENCODING_INT::INT8 ;
            }
            else if(a < INT16_MAX) {
                return ENCODING_INT::INT16 ;
            }
            else {
                return ENCODING_INT::INT32 ;
            }
        }
    }
    //以上都不满足,返回raw编码
    return STRING::REDIS_ENCODING_RAW ;
}

//创建rdb头部,redis标识，版本号
string rdb :: makeHeader() {
    string head ;
    //rdb文件标志
    head += "redis" ;
    head += "0001\r\n" ;   
    //rdb中database部分
    head += flag ;
    return head ;
}

bool rdb :: isNum(const char* num) {
    regex regFloat("^(-?\\d+)(\\.\\d+)?$");
    regex regInt("^-?\\d+$") ;
    bool res = regex_match(num, regFloat);
    if(res) {
        return res ;
    }   
    res = regex_match(num, regInt);
    if(res) {
        return res ;
    }   
    return false ;
}

int rdb :: getAllFileName(vector<string>&nameLs) {
    ifstream ifs("./.rdb/.redis_fileName", ios::in) ;
    if(ifs.fail()) {
        cout << __FILE__ << "   " << __LINE__ << endl ;
        return -1 ;
    }
    while(!ifs.eof()) {
        string file ;
        ifs >> file ;
        nameLs.push_back(file) ;
    }
    ifs.close() ;
    return 1 ;
}

char* rdb :: getFileInfo(const string s) {
    int fd = open(s.c_str(), O_RDONLY) ;
    if(fd < 0) {
        cout << __FILE__ << "   " << __LINE__ << endl ;
        return NULL ;
    }
    struct stat st;
    if(fstat(fd, &st) < 0) {
        cout << __FILE__ << "     " << __LINE__ << endl ;
        return NULL ;
    }
    char* buf = (char*)mmap(NULL, st.st_size, PROT_READ,MAP_SHARED, fd, 0) ;
    if(buf == NULL) {
        cout << __FILE__ << "     " << __LINE__ << endl ;
        return NULL ; 
    }
    close(fd) ;
    return buf ;
}

//初始化数据库
int rdb :: initRedis(cmdSet* cmdset) {
    vector<string>nameLs ;

    getAllFileName(nameLs) ;
    for(auto s: nameLs) {
        const char* buf = getFileInfo(s) ;   
        //文件信息
        string b = buf ;
        if(!isRedis(b.c_str()))  { 
            cout << __FILE__ << __LINE__ << endl ;
            return -1 ;
            //查找数据库编号
        }
        shared_ptr<redisDb>db = processFileInfo(b) ;   
        cmdset->append(db) ;
    }
   // vector<pair<int, shared_ptr<redisDb>>>dbLs ;
    /*string buf = buffer ;
    free(buffer) ;
    //获取buf中数据库所在位置
//    int index = buf.find("id:") ; 
    //获取数据库
    while(1) {

        //获取数据库编号
        int num = getNum(buf) ;
        //之前存在数据库中的编号相比于当前的最大的编号还大,扩增数据库
       /* if(num > cmdset->countRedis()) {
            cmdset->expend(num) ;
        }*/
        //获取到数据库
        //shared_ptr<redisDb>db = cmdset->getDB(num) ;
        //准备将数据保存保存到db中
        //long tim = getTime(buf) ;
        //当前时间大于文件中的时间
       /* if(tim < timer :: getCurTime()) {
            //处理掉数据
            continue ;           
        }
        int cmd = getCmd(buf) ;
        shared_ptr<dbObject> ob = cmdObject(buf, cmd, num)  ;  
        ob->setEndTime(tim) ;
        //将从文件中获取到的对象加入到db中
       // db->append(ob) ;
        //解析到最后退出
        if(buf.find("\r\n\r\n") == 0) {
            break ;
        }
    } */  
    //解析文件中的数据
    //验证是否有redis标识   
}

string rdb :: getValue(string& buf) {
    long index = buf.find(":") ;
    string value ;
    for(long i=0; buf[i] != '\r'; i++) {
        value += buf[i] ;
    }
    index = buf.find("\r\n") ;
    buf = buf.c_str() + index + 2 ;
    return buf ;
}

shared_ptr<dbObject> rdb :: cmdObject(string& buf, const int cmd, int num) {

    shared_ptr<dbObject> ob = factory::getObject("set")  ;  
    if(cmd == CMDTYPE :: STRING) {
        //是set　方法，找编码   
        int ecode = getEncoding(buf) ;
        string key = getKey(buf) ;
        ob->setName("set") ;
        ob->setKey(key) ;       
        ob->setType(type::DB_STRING) ;
        ob->setNum(num) ;
        if(ecode == STRING_ZIP::NO_CHANGE) {
            string value = getValue(buf) ;
            ob->setValue(value) ;
        }
        else if(ecode == ENCODING_INT::INT8) {
            string value = getValue(buf) ;
            ob->setValue(value) ;
        }
        else if(ecode == ENCODING_INT::INT16) {
            string value = getValue(buf) ;
            ob->setValue(value) ;
        }
        else if(ecode == ENCODING_INT::INT32) {
            string value = getValue(buf) ;
            ob->setValue(value) ;
        }

        else {
            //先获取原长
            int yc = getYc(buf) ;
            string value = getLzpValue(buf) ;
            //解码 
            char* val = NULL ;
            //还需要压缩前的长度
            if((val=(char*)malloc(yc)) == NULL) {
                cout <<__LINE__ << "    " << __FILE__ << endl ;
            }
            else {
                int ret = lzf_decompress(value.c_str(),(int)value.size(), val, yc) ;
                if(ret == 0) {
                    cout << __FILE__ << "   " << __LINE__ << endl ;
                }
                ob->setValue(val) ;
                //释放掉val
                free(val) ;
            }
        }
    }
    //其他一些命令
    else {
    }
    return ob ;
}

long rdb :: getYc(string&  buf) {
    long index = buf.find("yc:") ; 
    string yc ;
    for(long i=index+3; buf[i] != '\r'; i++) {
        yc += buf[i] ;
    }
    buf = buf.c_str() + 3 ;
    return atoi(yc.c_str()) ;
}

string rdb :: getLzpValue(string& buf) {
    long index = buf.find("\r\n") ;
    string value ;
    for(int i=index+2; buf[i] != '\r'; i++) {
        value += buf[i] ;
    }
    buf = buf.c_str() + index + 2 ;
    return buf ;
}

//获取字符串键
string rdb :: getKey(string& buf) {
    long index = buf.find("\r\n") ;
    string key ;
    for(long i=index+2; buf[i] != ':'; i++) {
        key += buf[i] ;
    }
    buf = buf.c_str() + index + 2;
    return key ;
}

int rdb :: getEncoding(string& buf) {
    long index = buf.find("ec") ;
    string code ;
    for(long i=index+2; buf[i] != '\r'; i++) {
        code +=  buf[i] ;
    }
    buf = buf.c_str() + index ;
    return atoi(code.c_str()) ;
}

int rdb :: getCmd(string& buf) {
    long index = buf.find("ctp:") ;
    string cmd ;
    for(long i=index+4; buf[i] != '\r'; i++) {
        cmd += buf[i] ;
    }
    buf = buf.c_str()+index ;
    return atoi(cmd.c_str()) ;
}

//获取过期时间
long rdb :: getTime(string& buf) {
    int index = buf.find("\r\ne:") ;
    string end ;
    for(long i=index; buf[i] != '\r'; i++) {
        end+=buf[i] ;
    }
    buf = buf.c_str() + index+2 ;
    return atoi(end.c_str()) ;
}

int rdb :: getNum(string& buf) {
    long index = buf.find("id:") ;
    string n ;
    for(long i=index+3; buf[i] != '\r'; i++) {
        n+=buf[i] ;
    }
    //将解析过的字符丢弃
    buf = buf.c_str() + index ;
    return atoi(n.c_str()) ;
}

bool rdb :: isRedis(const char* buffer) {
    string name ;
    for(long i=0; i<5; i++) {
        name+=buffer[i] ;
    }   
    if(name == "redis") {
        return true ;
    }
    return false ;
}

char* rdb :: getFileInfo() {
    //读数据
    ifstream in(".rdb/.redis_rdb", ios::in|ios::binary|ios::ate) ;
    if(in.fail()) {
        cout << __FILE__ <<"    " <<  __LINE__ << endl ;
        return NULL ;
    }
    //计算文件长度
    long l = in.tellg() ;
    //文件长度
    in.seekg(0, ios::beg) ;
    char* buffer = new char[l-1] ;
    in.read(buffer, l) ;
    in.close() ;
    return buffer ;
}
