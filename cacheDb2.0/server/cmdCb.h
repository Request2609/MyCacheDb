#pragma once
#include <iostream>
#include <memory>
#include <sys/mman.h>
#include <fstream>
#include <fcntl.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "dbObject.h"
#include "msg.pb.h"
#include "rdb.h"
#include "redisDb.h"

#define FLAG_FILE "FLAG_FILE" 

namespace type {
    //该键值是string 使用get　set 方法获取
    const int DB_STRING_ = 3 ;
    const int DB_HASH_ = 4 ;
    const int DB_LIST_ = 5 ;
} 
using namespace Messages ;
using namespace std ;
class dbObject ;
class factory ;
class redisDb ;


class cmdCb {
public:
    cmdCb() {}
    ~cmdCb() {}
    static int setLpush(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&tmp, shared_ptr<Response>& res) ;
    static int setHash(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&tmp, shared_ptr<Response>& res) ;
    static int save(vector<pair<int, shared_ptr<redisDb>>>&dbLs) ;
    static int isKeyExist(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&cmd) ;
    static int setCmd(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&tmp, shared_ptr<Response>& res);
    static int getCmd(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&tmp, shared_ptr<Response>& res) ;
    static int bgSave(vector<pair<int, shared_ptr<redisDb>>>&dbLs) ;
    static int setHget(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&tmp, shared_ptr<Response>& res) ;
    static char getFlag() ;
    static string lPop() ;
    static int lPop(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&tmp, shared_ptr<Response>& res) ;
    static string lPop(const string key, int num, int type) ;
    static vector<string>getList() ;
    static int setFlag(char c) ;
    int  blpop(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&tmp, shared_ptr<Response>&res) ;;

