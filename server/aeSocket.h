#pragma once
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/eventfd.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <functional>


class aeSocket {
    typedef std::function<void()>callBack ;
public:
    aeSocket() {sockFd = -1 ;}
    ~aeSocket() {close(sockFd) ;}
public :
    int anetCreateSocket() ;
    int setReuseAddr() ;
    int tcpServer(std::string port, std::string addr, int backLog) ;
    int getWritePairFd() {
        return fdPair[0] ;
    }
    int getReadPairFd() {
        return fdPair[1] ;
    }
    int acceptClient() ;
public :
    static int setNoBlocking(int fd) ;
    static int getReadFd() { return fdPair[0]; }
    static int getWriteFd() { return fdPair[1]; }
    static int createSocketPair() ;
    static int createEventFd() ;
private:
    static int eventFd ;
    static int fdPair[2] ;
    struct sockaddr_in serv ;
    int sockFd ; 
};

