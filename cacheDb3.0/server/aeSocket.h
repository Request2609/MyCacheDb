#pragma once
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <functional>
using namespace std ;
//封装redis ae socket

class aeSocket
{
    typedef function<void()>callBack ;
public:
    aeSocket() {sockFd = -1 ;}
    ~aeSocket() {close(sockFd) ;}
public :
    int anetCreateSocket() ;
    int setReuseAddr() ;
    int setNoBlocking(int fd) ;
    int tcpServer(string port, string addr, int backLog) ;
    int acceptClient() ;

private:
    struct sockaddr_in serv ;
    int sockFd ; 
};

