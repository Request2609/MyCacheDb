#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <iostream>
#include <sys/epoll.h>
#include <map>
#include "aeSocket.h"
#include "buffer.h"
#include "aeSocket.h"
using namespace std ;
#define SIZE 4096

//事件
namespace event {
    const int timeout = 1 ;
    const int commonMsg = 2 ;
}

//创建连接事件信息
class aeEvent :public enable_shared_from_this<aeEvent>{
    //创建文件描述符
    typedef function<int(shared_ptr<aeEvent>)> callBack ;
    
public :
    aeEvent() {
        //申请16个数据库
        //默认为０号数据库
        mask = 0 ;
    }
    ~aeEvent() {
    }
private :
    //套接字对象
    aeSocket sock ;
    int connFd ;
    //设置数据库的编号
    int num ;
    //标志位,是否为定时事件
    int mask ;
    //用户缓冲区
    buffer buf ;
    //读回调函数   
    callBack readFunc ;
    //写回调函数
    callBack writeFunc ;
    //epoll事件
    epoll_event* ev ;
    int servFd ;
public :
    void setMask(int mask) { this->mask = mask ; }
    int getMask() { return mask ; }
    int getNum() { return num ; }
    void setNum(int num) { this->num = num ; }
    int setNoBlock(int fd) { return sock.setNoBlocking(fd) ; }
    void setServFd(int fd) { servFd = fd ; }
    int getServFd() { return servFd ; }
    void setConnFd(int connFd) { this->connFd = connFd ; }
    int getConnFd() { return connFd ; }
    void setEvent(epoll_event* event) {ev = event ;}
    epoll_event* getEvent() { return ev; }
    aeSocket* getSocket() {   return &sock ; }
    int processRead() ;
    int processWrite() ;
    buffer* getBuf() { return &buf ; }
    void setReadCallBack(callBack cb) { readFunc = cb ;}
    void setWriteCallBack(callBack cb) { writeFunc = cb; }
} ;

