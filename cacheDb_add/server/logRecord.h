#pragma once
#include <iostream>
#include <memory>
#include <fcntl.h>
#include <vector>
#include <map>
#include <memory>
#include <sys/mman.h>
#include <string.h>
#include <mutex>
#include <cmath>
#include <unistd.h>
#include <condition_variable>
#include "redisDb.h"
#include "msg.pb.cpp"
#include "ThreadPool.h"
#include "cmdSet.h"
#include "logErr.h"
#include "easylogging++.h"

using Messages::Command ;
using namespace type ;
const int BUFFER_SIZE = 4096 ;
const int ERROR = 0 ; //错误日志
const int INFO = 0 ; //正常记录需要的信息
const int READ_ = 0 ;
const int WRITE_ = 1 ;
class threadPool ;
class logErr ;
class cmdSet ;

const int TYPESTRING = 1 ;
const int TYPEHASH = 2 ;
const int TYPELIST = 3 ;
const int OUT_WRITE = 1 ;
const int BLOCK_SIZE = 4096 ;
class logRecord {
public:
    ~logRecord() {}
    static void init() ;
    static void changeCommand(const std::shared_ptr<Command>& cmd) ;
    static void record(int num,string  buf) ;
    static void addRedisLog(int num) ;
    static void setcmdSet(cmdSet* cs) ;
    static int setRecordFileFd() ;
    //清空log文件
    static void clearLogFile(int num) ;
    //判断是否存在数据库编号为num的日志
    static long SUM ;
    static map<int, long>sizeMap ;
    static long MAX_FILE_SIZE ;
    static int recordFileFd ;
private:
    logRecord() {}
    bool isExistRedisLog(int num) ;
    void recordLog(int num, const char* buf) ;
    void addLog(int num) ;
    static std::shared_ptr<logRecord>getRecordObject() ;
    static std::shared_ptr<logRecord> lr ;
    static string formatStringAddLog(const std::shared_ptr<Command>&cmd, int& size) ;
    static string formatHashAddLog(const std::shared_ptr<Command>&cmd, int& size) ;
    static string formatListAddLog(const std::shared_ptr<Command>&cmd, int& size) ;
    //打开记录文件记录　日志文件所有的名称(有效的)，frozen logfile在断开后不会再次遍历了
    //添加记录文件的信息
    int addRecordFileName(string name) ;
    //各个数据库的编号
    std::map<int, int>ls ;
    //当前num号数据库缓存中的数据量第一个是数据库编号
    //第二个对中
    //一个是数据库中的当前记录的数据库中键值
    //数据的总子节数
    //第二个标志当前size是否可以修改，要是不可以，需要
    //等待条件变量
    //锁保护多线程sizeMap中的数据
    static mutex mute ;
    //static std::shared_ptr<threadPool>apool ;
    //发送信号，唤醒线程
    //第一个表示数据库编号，第二个表示data数据信息
    //这个map根据编号记录了日志
    vector<pair<int, string>>block ;
    static cmdSet* cset;
};

