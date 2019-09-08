#include "rdb.h"

//保存到文件
void rdb :: save(const shared_ptr<redisDb> db) {  
    //以二进制写形式创建rdb文件
    ofstream out(".rdb/.redis_rdb", ios::out|ios::binary|ios::trunc) ;
    if(out.fail()) {
        cout << __FILE__ << "    " << __LINE__ << endl ;
        return ;
    }
    int num = db->getId() ;
    //数据库编号
    string head = makeHeader() ;
    //将头部写入文件
    out << head ;
    //获取数据库中的对象元素
    shared_ptr<dbObject> rd = db->getNextDb() ;
    while(rd != nullptr) {
        string value = rd->getValue() ;
        //字符串类别是string
        if(rd->getType() == DB_STRING) {
            string key = rd->getKey() ;
            //设置数据库编号
            out << "id:" << num <<"\r\n" ;
            //将键的长度和键值写入文件
            //-1设置过期时间为永久
            out << "e:" ;
            //超时时间
            out << rd->getEndTime() << "\r\n" ;
            //值要根据不同的情况进行编码
            processString(key, out, value) ;
        }
        rd = db->getNextDb() ;
    }
    out.close() ;
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
            out << STRING_ZIP :: NO_CHANGE<<"\t\n" ;
            out << key <<":" << value;                 
        }
        else {
            //标志是否压缩过
            out << STRING_ZIP :: ENCODING_LZF <<"\t\n" ;
            out << key << ":" ;
            //压缩值
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

bool rdb :: isNum(string str) {
    stringstream sin(str);
    double d;
    char c;
    if(!(sin >> d))
        return false;
    if (sin >> c)
        return false;
    return true;
}

//初始化数据库
int rdb :: initRedis(shared_ptr<redisDb>& db) {
    
}
