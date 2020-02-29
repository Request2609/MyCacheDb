#pragma once
#include <iostream>
#include <memory>
#include <sys/epoll.h>
#include <functional>
#include "clientLoop.h"
#include "msg.pb.h"
#include "threadPool.h"
#include "syncQueue.h"
#include "aeEpoll.h"

#define BFSIZE 4096

class syncQueue ;
class threadPool ;
class clientLoop ;

class rpc {
    typedef std::function<int(int fd, std::vector<std::string>&ls, int num)> call ;
    typedef std::function<Messages::Response(std::string*)> parse ;
public:
    rpc() ;
    ~rpc() ;
public :
    Messages::Response getParseString(std::string* buff) ;
    //设置相应的callMethod方法
    void setCallMethod(call cb) ;
    //反序列化函数
    void setCallMethod(parse par) ; 
    int init() ;
    //ip和端口
    void setAddress(std::string ip, std::string port) { 
        ipPort.first = ip;  
        ipPort.second = port ;
    }
    int sendRequest(std::vector<std::string>&argStl, int num) ;
    int Connect(int& servFd) ;
    void disConnect() { 
        close(conFd) ; 
    }
    int getResponse() ;
    void setRdNum(int num) { 
        this->num = num ; 
    }
    static void readInfo(int fd) ;
private :
    clientSock client ;
    int conFd ;
    static std::shared_ptr<aeEpoll> aep ;   
    std::shared_ptr<syncQueue> que ;
    std::shared_ptr<threadPool> pool ;
    std::pair<std::string, std::string> ipPort ;
    std::shared_ptr<Messages::Command>cmd ;
    call request ;
    //反序列化函数
    parse parseMethod ;
    //数据库编号
    int num ;
};

