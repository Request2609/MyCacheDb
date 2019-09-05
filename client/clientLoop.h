#pragma once
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <functional>
#include <memory>
#include "stringOperation.h"
#include "readWrite.h"
#include "clientSock.h"
#include "aeEpoll.h"
#include "buffer.h"
#include "rpc.h"
#include "serializeString.h"
#define SIZE 4096
using namespace Messages ;
using namespace std ;
using namespace placeholders ;
class rpc ;

class clientLoop {
public:
    clientLoop(rpc* rc):stop(false) { 
        client = make_shared<clientSock>() ;
        //设置相应的rpc处理函数
        this->rc = rc ;
    }
    ~clientLoop() {}
public : 
    int recvInfo() ;
    void start(string ip, string port) ;
    int processMsg(Command& cmd, string& res) ;
    int sendRequest(string& res) ;
    int setEndSig() ;
private:
    bool stop ;
    shared_ptr<clientSock> client;
    buffer bf ;
    rpc *rc ;
    vector<string> cmdStl ;
    int servFd ;
    int num ;
};

