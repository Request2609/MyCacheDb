#pragma once
#include <iostream>
#include <map>
#include <memory>   
#include <vector>
#include <string> 
#include "timerHeap.h"
#include "aeEvent.h"
#include "buffer.h"
#include "backInfo.h"
#include "redisDb.h"
#include "cmdSet.h"
#include "serializeParse.h"
#include "readWrite.h"
#include "rpc.h"
#include "msg.pb.h"
#include "aofRecord.h"

#define SIZE 4096

class rpc ;
class cmdSet ;
class redisCommand ;
class cmdSet ;
class backInfo ;
class listWaitQueue ;
class aeEvent ;
class listWaitQueue ;
class aofRecord ;

//错误类型
enum {
    NOT_FOUND = 1,
    FOUND_ = 2,
    KEY_INVALID = 3,
    SUCESS_ = 4, 
    PROCESS_ERROR = -1
} ; 

//处理消息请求的函数
class cmdProcess {
    typedef std::function<void(std::shared_ptr<Response>res, int fd)> request ;
    typedef std::function<void(void)> task ;
public :
    //创建
    //消息处理
    cmdProcess() {
        cmdSet_ = make_shared<cmdSet>() ;
        //获取命令表
        rc = make_shared<rpc>() ;
    }
    ~cmdProcess() {}
public :
    void initCmdCb() ;
    //时间事件回调
public :
    int initRedis() ;
    void setRpc(std::shared_ptr<rpc>rc) { 
        this->rc = rc ; 
    }
    void setRpcMethod() ;
    int processMsg(std::shared_ptr<aeEvent>&tmp) ;
    int sendMsg(std::shared_ptr<aeEvent>tmp) ;
    void getAfterKeyPart() ;
    int findCmd(std::shared_ptr<Messages::Command>tmp) ;
private :
    //命令集
    //键值以后的部分成员
    std::shared_ptr<cmdSet> cmdSet_ ;
    std::vector<std::string> lastKey ;   
    //rpc,主要进行序列化和反序列化
    std::shared_ptr<rpc> rc ;
};

