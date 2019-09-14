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
    const char* h =head.c_str() ;
    out << h ;
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
            out << "ctp:" << DB_STRING << "\r\n" ;
            //值要根据不同的情况进行编码
            processString(key, out, value) ;
        }
        rd = db->getNextDb() ;
    }
    out << "\r\n" ;  
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
        out << "ec:" << STRING :: REDIS_ENCODING_INT << "\r\n" ;
        //值
        int8_t a = atoi(v) ;
        //将键值写入到文件中
        out << key <<":" << a << "\r$\n" ;
    }
    else if(type == ENCODING_INT::INT16) {
        out << "ec:" << STRING :: REDIS_ENCODING_INT << "\r\n" ;
        //值
        int16_t a = atoi(v) ;
        //将键值写入到文件中
        out << key <<":" << a << "\r$\n" ;
        
    }
    else if(type == ENCODING_INT::INT32) {
        out << "ec:" << ENCODING_INT::INT8 << "\r\n" ;
        //值
        int32_t a = atoi(v) ;
        //将键值写入到文件中
        out << key <<":" << a << "\r$\n" ;
    }
    else if(type == STRING::REDIS_ENCODING_RAW) {
        out << "ec:" << STRING::REDIS_ENCODING_RAW << "\r\n" ;
        //值
        int size = value.size() ;
        if(size <= 20) {
            out << "yc:" << size <<"\r\n" ;
            out << key <<":" << value<< "\r$\n";                 
        }
        else {
            out << "yc:" << size << "\r\n" ;
            //原长写入文件
            //标志是否压缩过
            //压缩值
            //将字符串原长保存到文件
            int ll = 0 ;
            void* ot = lzfCompress(value, ll) ;
            if(ot == NULL) {
                cout << "无效数据"<< endl ;
            }
            string a = (char*)ot ;
            out << "xc:" << ll << "\r\n" ;
            out << key << ":" ;
            cout << "压缩的数据。。。。。"<< a << endl ;
            out << a <<"\r$\n" ;
            //销毁ot内存
            free(ot) ;
        }
    }
}

//压缩
void* rdb :: lzfCompress(const string& value, int& ll) {
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
    ll = l ;
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
    ifstream ifs(".rdb/.redis_fileName", ios::in) ;
    if(ifs.fail()) {
        cout << __FILE__ << "   " << __LINE__ <<"   "<<  strerror(errno)<< endl ;
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

string rdb :: getFileInfo(const string s) {
    long l = 0 ;
    int fd = open(s.c_str(), O_RDWR|O_CREAT) ;
    if(fd < 0) {
        cout << __LINE__ << "     " << __FILE__ << endl ;
        return "" ;
    }
    string b ;
    char buf[SIZE_::size] ;
    int n= 0;
    while(1) {
        n=read(fd, buf, sizeof(buf)) ;
        if(n < 0) {
            cout << __FILE__ << "   "  << __LINE__ << endl ;
            return "" ;
        }
        if(n == 0) {
            break ;
        }
        b+=buf ;
    }
    close(fd) ;
    return b ;
}

//初始化数据库
int rdb :: initRedis(cmdSet* cmdset) {

    vector<string>nameLs ;
    int i = getAllFileName(nameLs) ;
    if(nameLs.size() == 0|| i < 0) {
        return -1 ;
    }
    for(auto s: nameLs) {
        if(s.size() == 0) {
            continue ;
        }
        string str = getFileInfo(s) ;   
        if(str.empty()) {
            return -1 ;
        } 
        //文件信息
        shared_ptr<redisDb>db = recoverDb :: recover(str, cmdset) ;   
    }
    return 1 ;
}

