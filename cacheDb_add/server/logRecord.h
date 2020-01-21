#pragma once
#include <iostream>
#include <memory>
#include <fcntl.h>
#include <vector>
#include <map>
#include <sys/mman.h>
#include <cmath>
#include <unistd.h>
#include "redisDb.h"
#include "msg.pb.cpp"
#include "ThreadPool.h"
#include "logErr.h"
#include "easylogging++.h"

using Messages::Command ;
using namespace type ;
const int BUFFER_SIZE = 4096 ;
const int ERROR = 0 ; //错误日志
const int INFO = 0 ; //正常记录需要的信息
const int READ = 0 ;
const int WRITE = 1 ;
class threadPool ;
class logErr ;
class logRecord {
public:
    ~logRecord() {}
    static void init() ;
    static void changeCommand(const std::shared_ptr<Command>& cmd) ;
    static void record(int num, const string  buf) ;
    static void addRedisLog(int num) ;
    //判断是否存在数据库编号为num的日志
private:
    logRecord() {}
    bool isExistRedisLog(int num) ;
    void recordLog(int num, const char* buf) ;
    void addLog(int num) ;
    static std::shared_ptr<logRecord>getRecordObject() ;
    static std::shared_ptr<logRecord> lr ;
    static string formatStringAddLog(const std::shared_ptr<Command>&cmd) ;
    static string formatHashAddLog(const std::shared_ptr<Command>&cmd) ;
    static string formatListAddLog(const std::shared_ptr<Command>&cmd) ;
    //各个数据库的编号
    std::map<int, int>ls ;
    static std::shared_ptr<threadPool>pool ;
    static long MAX_FILE_SIZE ;
};

