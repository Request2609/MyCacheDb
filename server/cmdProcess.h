#pragma once
#include <map>
#include <memory>   
#include <vector>
#include <string>
#include "aeEvent.h"
#include "buffer.h"
#include "backInfo.h"
#include "redisDb.h"
#include "cmdSet.h"
#include "serializeParse.h"
#include "readWrite.h"
#include "rpc.h"
#define SIZE 4096
using namespace std ;

class aeEvent ;
class rpc ;
class cmdSet ;
class redisCommand ;

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
    typedef function<void(shared_ptr<Response>res, int fd)> request ;
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
    void setRpc(shared_ptr<rpc>rc) { this->rc = rc ; }
    void setRpcMethod() ;
    int processMsg(shared_ptr<aeEvent>&tmp) ;
    int sendMsg(shared_ptr<aeEvent>tmp) ;
    void getAfterKeyPart() ;
    int findCmd(const shared_ptr<Command>tmp) ;
private :
    //命令集
    //键值以后的部分成员
    shared_ptr<cmdSet> cmdSet_ ;
    vector<string> lastKey ;   
    //rpc,主要进行序列化和反序列化
    shared_ptr<rpc> rc ;
};

