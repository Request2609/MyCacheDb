#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <map>
#include <vector>
#include <arpa/inet.h>
 #include<netinet/tcp.h>
#include <memory>
#include "cmdProcess.h"
#include "msg.pb.h"
#include "syncQueue.h"
#include "clientLoop.h"

#define REQ_SIZE 4096 
class cmds ;
class cmdProcess ;
class syncQueue ;
class clientLoop ;

class cmds {
public:
    cmds() ;
    ~cmds() ;
public :
    //一个命令
    std::map<std::string, int>cmdList ;
public :
    void build() ;
    int isCmdExist(std::string cmd) ;
};

class request {
public :
    static int isConnect(int fd) ;
    static int sendAfterSerial(int fd, Messages::Command& cmd) ;
    static int sendReq(int fd, std::vector<std::string>&res, int num) ;
    static int processCmd(std::vector<std::string>&res, 
                          Messages::Command&com) ;
private :
    static int num ;
} ;
