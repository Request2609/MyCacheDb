#pragma once
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <cstdarg>
#include <readline/history.h>
#include <iostream>
#include <functional>
#include <memory>
#include <fstream>
#include "readWrite.h"
#include "clientSock.h"
#include "aeEpoll.h"
#include "buffer.h"
#include "rpc.h"
#include "serializeString.h"
#include "threadPool.h"
#include "request.h"

class syncQueue ;
#define SIZE 4096
#define END  "\r\n" 
class rpc ;
class cmds ;
class syncQueue ;

class clientLoop {
public:
    clientLoop():stop(false) { 
        //设置相应的rpc处理函数
        num = 0 ;
        servFd = -1 ;
    }
    ~clientLoop() {
        if(servFd > 0) {
            close(servFd) ;
        }
    }
public : 
    std::vector<std::string> split(const std::string &s, 
                                   const std::string &seperator) ;
    int recvInfo() ;
    void start() ;
    int processMsg(Messages::Command& cmd, std::string& res) ;
    int sendRequest(std::string& res) ;
    int setEndSig() ;
    int getIpPort() ;
    int init() ;
    int init(std::string ip, std::string port) ;
    int sendRequest(std::string type, ...) ;
    int getResult() ;
    static int mode ;
private:
    std::shared_ptr<cmds>cmd ;
    std::string port ;
    std::string ip ;
    bool stop ;
    std::shared_ptr<clientSock> client;
    buffer bf ;
    std::shared_ptr<rpc>rc;
    std::vector<std::string> cmdStl ;
    int servFd ;
    int num ;
};

