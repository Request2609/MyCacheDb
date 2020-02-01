#pragma once
#include <iostream>
#include <memory>
#include <unistd.h>
#include <regex>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sstream>
#include <fstream>
#include <fcntl.h>
#include <sys/stat.h>
#include <cctype>//判断字符类型需要的头文件
#include "lzf.h"
#include "lzfP.h"
#include "redisDb.h"
#include "cmdSet.h"
#include "timer.h"
#include "recoverDb.h"
//#include "easylogging++.h"
//#include "logErr.h"

class logErr ;
class cmdSet ;
class rdb ;
const int version = 1 ;
//selected 条目
const char flag = 0 ;

using namespace std ;
using namespace type ;
//创建rdb持久化机制

//字符串的两种编码
namespace STRING {
    //string 保存的是长度不超过32的整数
    const int REDIS_ENCODING_INT = 1 ;
    //string 保存
    const int REDIS_ENCODING_RAW = 2 ;
}

namespace SIZE_ {
    const int size = 4096 ;
}

namespace CMDTYPE {
    const int STRING = 3 ;
    const int DB_HASH = 4 ;
} 

//不同位数的int类型
namespace ENCODING_INT {
    const int INT8 = 5 ;
    const int INT16 = 6 ;
    const int INT32 = 7 ;
}

//字符串的两种压缩方式
namespace STRING_ZIP {
    //适合于字符串长度小于20字节
    const int NO_CHANGE = 8 ;
    //适合字符串长度大于20字节
    const int ENCODING_LZF = 9 ;
}

namespace cmdName {
    const string set = "set" ;
    const string get = "get" ;
}

class rdb {
public:
    rdb() {}
    ~rdb() {}
    //判断字符串是否是数字
    static string lzfCompress(const string value, int& len) ;
    static bool isNum(const char* num) ;
    //获取字符串的编码类型
    static int getStringEncodingType(const string value) ;
    static string makeHeader() ;
    static shared_ptr<redisDb> processFileInfo(string& s) ;
    static int save(const shared_ptr<redisDb> db, char* fileName) ;
    static int  initRedis(cmdSet* cmdset) ;
    static string  getFileInfo(const string s) ;
    static int getAllFileName(vector<string>& nameLs) ;
    static bool isRedis(const char* buffer) ;
    static string tmpFileName(const char* fileName) ;
    static int getLogFileName(vector<string>&logName) ;
    static string readLogFile(const string& file) ; 
public :
    static void processString(const string key, ofstream& aa, const string value) ;
    static void processHash(ofstream& aa, const shared_ptr<dbObject>rd) ;
    static void processList(ofstream& aa, const shared_ptr<dbObject>rd) ;
} ;
