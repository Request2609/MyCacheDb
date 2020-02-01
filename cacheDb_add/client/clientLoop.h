#pragma once
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <cstdarg>
#include <readline/history.h>
#include <iostream>
#include <functional>
#include <memory>
#include "readWrite.h"
#include "clientSock.h"
#include "aeEpoll.h"
#include "buffer.h"
#include "rpc.h"
#include "serializeString.h"
#include "threadPool.h"
#include "request.h"
#define SIZE 4096
using namespace Messages ;
using namespace placeholders ;
using namespace std ;
class rpc ;
class cmds ;
class threadpool ;
const char* END = "\r\n" ;
class clientLoop {
public:
    clientLoop():stop(false) { 
        //设置相应的rpc处理函数
        num = 0 ;
    }
    ~clientLoop() {
        if(servFd < 0) {
            close(servFd) ;
        }
    }
public : 
    vector<string> split(const string &s, const string &seperator) ;
    int recvInfo() ;
    void start() ;
    int processMsg(Command& cmd, string& res) ;
    int sendRequest(string& res) ;
    int setEndSig() ;
    void init(string ip, string port) ;
    void sendRequest(string type, ...) ;
private:
    shared_ptr<cmds>cmd ;
    string port ;
    string ip ;
    shared_ptr<threadpool> pool ;
    bool stop ;
    shared_ptr<clientSock> client;
    buffer bf ;
    shared_ptr<rpc>rc;
    vector<string> cmdStl ;
    int servFd ;
    int num ;
};

