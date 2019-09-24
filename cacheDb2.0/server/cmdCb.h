#pragma once
#include <iostream>
#include <memory>
#include <sys/mman.h>
#include <fstream>
#include <fcntl.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "msg.pb.h"
#include "redisDb.h"
#include "rdb.h"

#define FLAG_FILE "FLAG_FILE" 

using namespace Messages ;
using namespace std ;

class dbObject ;
class factory ;
class redisDb ;


class cmdCb {
public:
    cmdCb() {}
    ~cmdCb() {}
    static int setHash(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&tmp, shared_ptr<Response>& res) ;
    static int save(vector<pair<int, shared_ptr<redisDb>>>&dbLs) ;
    static int isKeyExist(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&cmd) ;
    static int setCmd(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&tmp, shared_ptr<Response>& res);
    static int getCmd(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&tmp, shared_ptr<Response>& res) ;
    static int bgSave(vector<pair<int, shared_ptr<redisDb>>>&dbLs) ;
    static int setHget(shared_ptr<redisDb>&wcmd, shared_ptr<Command>&tmp, shared_ptr<Response>& res) ;
    static char getFlag() ;
    static int setFlag(char c) ;
};

