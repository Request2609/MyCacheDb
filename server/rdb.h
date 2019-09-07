#pragma once
#include <iostream>
#include <memory>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <fcntl.h>
#include <cctype>//判断字符类型需要的头文
#include "lzf.h"
#include "lzfP.h"
#include "cmdSet.h"
#include "redisDb.h"

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

//不同位数的int类型
namespace ENCODING_INT {
    const int INT8 = 1 ;
    const int INT16 = 2 ;
    const int INT32 = 3 ;
}

//字符串的两种压缩方式
namespace STRING_ZIP {
    //适合于字符串长度小于20字节
    const int NO_CHANGE = 1 ;
    //适合字符串长度大于20字节
    const int ENCODING_LZF = 2 ;
}

class rdb {
public:
    rdb() {}
    ~rdb() {}
    //判断字符串是否是数字
    static void * lzfCompress(const string& value) ;
    static bool isNum(string str) ;
    //获取字符串的编码类型
    static int getStringEncodingType(const string value) ;
    static void processString(const string key, ofstream& out, const string value) ;
    static string makeHeader() ;
    static void save(const shared_ptr<redisDb> db) ;
private :
};

