#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <vector>
#include <arpa/inet.h>
 #include<netinet/tcp.h>
#include <memory>
#include "cmdProcess.h"
#include "msg.pb.h"
#include "syncQueue.h"
#include "clientLoop.h"

#define REQ_SIZE 4096 
using namespace std ;
using namespace Messages ;
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
    map<string, int>cmdList ;
public :
    void build() ;
    int isCmdExist(string cmd) ;
};

class request {
public :
    static int isConnect(int fd) ;
    static int sendAfterSerial(int fd, Command& cmd) ;
    static int sendReq(int fd, vector<string>&res, int num) ;
    static int processCmd(vector<string>&res, Command&com) ;
private :
    static int num ;
} ;
