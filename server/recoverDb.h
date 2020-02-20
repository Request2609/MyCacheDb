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
    static bool  isRedis(std::string& s) ;
    static std::shared_ptr<redisDb> recover(std::string& s, cmdSet* cmdset) ;
    static int getListObject(std::string& s, std::shared_ptr<dbObject>& ob) ;
    static void getHead(std::string& s) ;
    static int getNum(std::string& s) ;
    static long getTime(std::string& buf) ;
    static int getType(std::string& s) ;
    static std::string getListValues(std::string& s) ;
    static int getEncoding(std::string& s) ;
    static std::string getKey(std::string& s, std::string& value) ;
    static void stringGet(std::string& s, std::shared_ptr<dbObject>& ob) ;
    static long getYC(std::string& buf) ;
    static std::string  getValue(std::string& s) ;
    static std::string getKey(std::string& s) ;
    static int getXC(std::string& s) ;
    static int  hashGet(std::string& s, std::shared_ptr<dbObject>&ob) ;
    static std::string getHashKey(std::string& s) ;
    static std::pair<std::string, std::string> getAttrKV(std::string& s) ;
    static bool isOk(long index)  ;
    static void recoverByLog(std::vector<std::pair<int, std::shared_ptr<redisDb>>>* dbLs) ;
    static void parseAndExecAofCmd(const char*, std::vector<std::pair<int, std::shared_ptr<redisDb>>>&) ;
    static std::vector<std::string> split(const std::string &s, 
                                const std::string &seperator) ;
    static int aofString(std::shared_ptr<Command>&cmd, const std::vector<std::string>& str) ;
    static int aofHash(std::shared_ptr<Command>&cmd, const std::vector<std::string>& str) ;
    static int aofList(std::shared_ptr<Command>&cmd, const std::vector<std::string>& str) ;
    static int aofSortSet(std::shared_ptr<Command>&cmd, const std::vector<std::string>& str ) ;
    static int aofSet(std::shared_ptr<Command>&cmd, const std::vector<std::string>& str ) ;
    static std::shared_ptr<redisDb> getDb(int num, const std::vector<std::pair<int, std::shared_ptr<redisDb>>>&) ;
    static int getTime() ;
};

