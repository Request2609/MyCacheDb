#pragma once
#include <iostream>
#include <memory>
#include <sys/epoll.h>
#include <functional>
#include "clientLoop.h"
#include "msg.pb.h"
#include "ThreadPool.h"
#include "syncQueue.h"
#include "aeEpoll.h"

#define BFSIZE 4096
using namespace Messages ;
using namespace std ;

class syncQueue ;
class threadPool ;
class clientLoop ;

class rpc {
    typedef function<int(int fd, vector<string>&ls, int num)> call ;
    typedef function<Response(string*)> parse ;
public:
    rpc() ;
    ~rpc() ;
public :
    Response getParseString(string* buff) ;
    //设置相应的callMethod方法
    void setCallMethod(call cb) ;
    //反序列化函数
    void setCallMethod(parse par) ; 
    int init() ;
    //ip和端口
    void setAddress(string ip, string port) { ipPort.first = ip;  ipPort.second = port ;}
    int sendRequest(vector<string>&argStl, int num) ;
    int Connect(int& servFd) ;
    void disConnect() { close(conFd) ; }
    int getResponse() ;
    void setRdNum(int num) { this->num = num ; }
    static void readInfo(int fd) ;
private :
    clientSock client ;
    int conFd ;
    static shared_ptr<aeEpoll> aep ;   
    shared_ptr<syncQueue> que ;
    shared_ptr<threadPool> pool ;
    pair<string, string> ipPort ;
    shared_ptr<Command>cmd ;
    call request ;
    //反序列化函数
    parse parseMethod ;
    //数据库编号
    int num ;
};

