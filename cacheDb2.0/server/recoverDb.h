#pragma once
#include <iostream>
#include "rdb.h"
#include "redisDb.h"
#include "cmdSet.h"
using namespace std ;

class cmdSet ;
//字符串的两种编码
namespace STRING_ {
    //string 保存的是长度不超过32的整数
    const int REDIS_ENCODING_INT = 1 ;
    //string 保存
    const int REDIS_ENCODING_RAW = 2 ;
}

namespace CMDTYPE_ {
    const int STRING = 3 ;
    const int DB_HASH= 4 ;
} 

class recoverDb {
public:
    recoverDb() {}
    ~recoverDb() {}
    static bool  isRedis(string& s) ;
    static shared_ptr<redisDb> recover(string& s, cmdSet* cmdset) ;
    static void getHead(string& s) ;
    static int getNum(string& s) ;
    static long getTime(string& buf) ;
    static int getType(string& s) ;
    static int getEncoding(string& s) ;
    static string getKey(string& s, string& value) ;
    static void stringGet(string& s, shared_ptr<dbObject>& ob) ;
    static long getYC(string& buf) ;
    static string  getValue(string& s) ;
    static string getKey(string& s) ;
    static int getXC(string& s) ;
    static int  hashGet(string& s, shared_ptr<dbObject>&ob) ;
    static string getHashKey(string& s) ;
    static pair<string, string> getAttrKV(string& s) ;
    static bool isOk(long index)  ;
};

