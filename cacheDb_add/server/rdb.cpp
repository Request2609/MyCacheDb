#include "rdb.h"

string rdb :: tmpFileName(const char* fileName) {
    string tmp = fileName ;
    tmp += ".tmp" ;
    return tmp ;
}


//保存到文件
int rdb :: save(const shared_ptr<redisDb> db, char* fileName) {     
    //获取数据库文件
    //string ss = tmpFileName(fileName) ;  
    ofstream out(fileName, ios::out|ios::binary|ios::trunc|ios::app) ;
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
        string value ;
        int type = rd->getType() ;
        //字符串类别是string
        out << "e:" ;
        out << rd->getEndTime() << "\r\n" ;
        if(type == DB_STRING) {
            string value = rd->getValue() ; 
            string key = rd->getKey() ;
            out << "ctp:" << DB_STRING << "\r\n" ;
            processString(key, out, value) ;
        }
        //hash的保存
        if(type == DB_HASH) {
            string key = rd->getKey() ;
        //    out << rd->getEndTime()<<"\r\n" ;
            out << "ctp:" << DB_HASH << "\r\n" ;
            processHash(out, rd) ;         
        }
        //是链表
        if(type == DB_LIST) {
          //  out << rd->getEndTime() << "\r\n" ;
            out  <<"ctp:" << DB_LIST << "\r\n" ;
            processList(out, rd) ;
        }   
        rd = db->getNextDb() ;
    }
    out << "\r\n" ;  
    out.close() ;
    return 1 ;
}

void rdb :: processList(ofstream& aa, const shared_ptr<dbObject>rd) {
    string key = rd->getKey() ;
    aa << key << "\r\n" ;
    vector<string> ls = rd->getValues("") ;
    int len = ls.size() ;
    for(int i=0; i<len; i++) {
        aa <<ls[i]<< "\r\n" ;
    }
    aa << "\r$\n" ;
}


void rdb :: processHash(ofstream& out, const shared_ptr<dbObject>rd) {

    string key = rd->getKey() ;
    string value="" ;
    int len = 0 ;
    out << key << "\r\n" ;
    //获取hash中的键值属性
    while(1) {
        value = rd->getValue() ;
        len =value.size() ;
        if(len == 0) {
            break ;
        }
        int index = value.find("\r\n") ;
        string key = value.substr(0, index) ;
        string val = value.substr(index+2, len-index) ;
        out << key << ":" << val << "\r\n" ;
    }
    out <<"\r$\n" ;
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
            
            string a = lzfCompress(value, ll);
            if(a.empty()) {
                ll = value.size() ;
                a = value ;
            }
            out << "xc:" << ll << "\r\n" ;
            out << key << ":" ;
            out << a <<"\r$\n" ;
        }
    }   
}

string rdb :: lzfCompress(string value, int& ll) {
    size_t len = value.size();  // 字符串未压缩前的长度
    size_t comprlen;  // 压缩后的长度
    size_t outlen;    // 输出缓存的最大长度
    void *out;
    //当字节至少四字节长以上时才能压缩
    if (len <= 4) {
        return "";
    }
    
    outlen = len-4;
    
    if ((out = malloc(outlen+3)) == NULL) 
    {
        cout << "out = malloc(outlen+1)" << endl;
        return "";
    }
    //传入数据，数据长度，任意类型的指针
    comprlen = lzf_compress(value.data(), len, out, outlen);  
    if (comprlen == 0) 
    {
        cout << "压缩失败！"<< endl ;
        free(out);
        return "";
    }
    ll = comprlen ;
    string a = (char*)out ;
    free(out) ;
    return a ;
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
    int fd = open(s.c_str(), O_RDWR) ;
    if(fd < 0) {
        cout << strerror(errno) << "     " << __LINE__ << "     " << __FILE__ << endl ;
        return "" ;
    }
    struct stat st ;
    int ret = fstat(fd, &st) ;
    if(st.st_size == 0) {
        cout << "数据库文件为空！" << endl ;
        return "" ;
    }
    if(ret < 0) {
        cout << __FILE__ << "       " << __LINE__ << endl ;
        return "" ;
    }
    long size = st.st_size ;
    if(size%4096 != 0) {
        size = (size/4096+1)*4096 ;
    }
    //内存映射
    char* flag = (char*)mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0) ;
    if(flag == NULL) {
            cout << __FILE__ << "    " << __LINE__ << endl ;
            return "" ;
    }
    close(fd) ;
    string b = flag ;
    munmap(flag, size) ;
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
        shared_ptr<redisDb>db = recoverDb :: recover(str, cmdset) ;   
    }
    vector<string>logName ;
    getLogFileName(logName);
    int len = logName.size() ;
    //恢复数据库
    for(int i=0; i<len; i++) {
           
    }
    return 1 ;
}

int rdb::getLogFileName(vector<string>&logName) {
    ifstream in("../logInfo/allLogFileName", ios::in) ;
    if(in.fail()) {
        string error = __FILE__  ;
        error+=strerror(errno);
        logErr::record(error) ;
        return -1 ;   
    }
    while(!in.eof()) {
        string s = "" ;
        in>> s ;
        logName.push_back(s) ;
    }
    in.close() ;
    return 1 ;
}
