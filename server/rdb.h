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
#include "recoverDb.h"
#include "dbObject.h"

class dbObject ;
class cmdSet ;
class rdb ;
class redisDb ;
const int version = 1 ;
//selected 条目
const char flag = 0 ;

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
    const std::string set = "set" ;
    const std::string get = "get" ;
}

class rdb {
public:
    rdb() {}
    ~rdb() {}
    //判断字符串是否是数字
    static std::string lzfCompress(const std::string value, int& len) ;
    static bool isNum(const char* num) ;
    //获取字符串的编码类型
    static int getStringEncodingType(const std::string value) ;
    static std::string makeHeader() ;
    static std::shared_ptr<redisDb> processFileInfo(std::string& s) ;
    static int save(const std::shared_ptr<redisDb> db, char* fileName) ;
    static int  initRedis(cmdSet* cmdset) ;
    static std::string  getFileInfo(const std::string s) ;
    static int getAllFileName(std::vector<std::string>& nameLs) ;
    static bool isRedis(const char* buffer) ;
    static std::string tmpFileName(const char* fileName) ;
    static int getLogFileName(std::vector<std::string>&logName) ;
    static std::string readLogFile(const std::string& file) ; 
public :
    static void processString(const std::string key, std::ofstream& aa, const std::string value) ;
    static void processHash(std::ofstream& aa, const std::shared_ptr<dbObject>rd) ;
    static void processList(std::ofstream& aa, const std::shared_ptr<dbObject>rd) ;
} ;
