#pragma once
#include <iostream>
#include <memory>
#include <sys/mman.h>
#include <fstream>
#include <fcntl.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "dbObject.h"
#include "rdb.h"
#include "redisDb.h"
#include "msg.pb.h"
#include "aofRecord.h"
#include "msg.pb.h"

#define FLAG_FILE "FLAG_FILE" 

namespace type {
    //该键值是string 使用get　set 方法获取
    const int DB_STRING_ = 3 ;
    const int DB_HASH_ = 4 ;
    const int DB_LIST_ = 5 ;
} 

class dbObject ;
class factory ;
class redisDb ;
class aofRecord ;

class cmdCb {
public:
    cmdCb() {}
    ~cmdCb() {}
    static int setLpush(std::shared_ptr<redisDb>&wcmd, std::shared_ptr<Messages::Command>&tmp, std::shared_ptr<Messages::Response>& res) ;
    static int setHash(std::shared_ptr<redisDb>&wcmd, std::shared_ptr<Messages::Command>&tmp, std::shared_ptr<Messages::Response>& res) ;
    static int save(std::vector<std::pair<int, std::shared_ptr<redisDb>>>&dbLs) ;
    static int isKeyExist(std::shared_ptr<redisDb>&wcmd, std::shared_ptr<Messages::Command>&cmd) ;
    static int setCmd(std::shared_ptr<redisDb>&wcmd, std::shared_ptr<Messages::Command>&tmp, std::shared_ptr<Messages::Response>& res);
    static int getCmd(std::shared_ptr<redisDb>&wcmd, std::shared_ptr<Messages::Command>&tmp, std::shared_ptr<Messages::Response>& res) ;
    static int bgSave(std::vector<std::pair<int, std::shared_ptr<redisDb>>>&dbLs) ;
    static int setHget(std::shared_ptr<redisDb>&wcmd, std::shared_ptr<Messages::Command>&tmp, std::shared_ptr<Messages::Response>& res) ;
    static int lPop(std::shared_ptr<redisDb>&wcmd, std::shared_ptr<Messages::Command>&tmp, std::shared_ptr<Messages::Response>& res) ;
    static std::vector<std::string>getList() ;
    static int  blPop(std::shared_ptr<redisDb>&wcmd, std::shared_ptr<Messages::Command>&tmp, std::shared_ptr<Messages::Response>&res) ;
    static int sortSetAdd(std::shared_ptr<redisDb>&wcmd, std::shared_ptr<Messages::Command>&tmp, std::shared_ptr<Messages::Response>&res) ;
    static int sortSetGetMember(std::shared_ptr<redisDb>&wcmd, std::shared_ptr<Messages::Command>&tmp, std::shared_ptr<Messages::Response>&res) ;
    static int setSetValue(std::shared_ptr<redisDb>&wcmd, std::shared_ptr<Messages::Command>&tmp, std::shared_ptr<Messages::Response>&res) ;
    static int sPop(std::shared_ptr<redisDb>&wcmd, std::shared_ptr<Messages::Command>&tmp, std::shared_ptr<Messages::Response>&res) ;
};
