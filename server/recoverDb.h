#pragma once
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string>
#include <string.h>
#include "rdb.h"
#include "redisDb.h"
#include "cmdSet.h"
#include "dbObject.h"
#include "msg.pb.h"
#include "cmdCb.h" 

using namespace std ;
using namespace Messages ;
class cmdSet ;
class redisDb ;
class dbObject ;
class cmdCb ;
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
    const int DB_LIST = 5 ;
} 

class recoverDb {
public:
    recoverDb() {}
    ~recoverDb() {}
    static bool  isRedis(string& s) ;
    static shared_ptr<redisDb> recover(string& s, cmdSet* cmdset) ;
    static int getListObject(string& s, shared_ptr<dbObject>& ob) ;
    static void getHead(string& s) ;
    static int getNum(string& s) ;
    static long getTime(string& buf) ;
    static int getType(string& s) ;
    static string getListValues(string& s) ;
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
    static void recoverByLog(vector<pair<int, shared_ptr<redisDb>>>* dbLs) ;
    static void parseAndExecAofCmd(const char*, vector<pair<int, shared_ptr<redisDb>>>&) ;
    static vector<string> split(const string &s, 
                                const string &seperator) ;
    static int aofString(shared_ptr<Command>&cmd, const vector<string>& str) ;
    static int aofHash(shared_ptr<Command>&cmd, const vector<string>& str) ;
    static int aofList(shared_ptr<Command>&cmd, const vector<string>& str) ;
    static int aofSortSet(shared_ptr<Command>&cmd, const vector<string>& str ) ;
    static int aofSet(shared_ptr<Command>&cmd, const vector<string>& str ) ;
    static shared_ptr<redisDb> getDb(int num, const vector<pair<int, shared_ptr<redisDb>>>&) ;
    static int getTime() ;
};

