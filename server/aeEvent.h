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
#include "aeEpoll.h"
#include "timerHeap.h"

#define SIZE 4096

class TimerManager ;
//事件
namespace event {
    const int timeout = 1 ;
    const int commonMsg = 2 ;
}

//创建连接事件信息
class aeEvent :public std::enable_shared_from_this<aeEvent>{
    //创建文件描述符
    typedef std::function<int(std::shared_ptr<aeEvent>)> callBack ;
    
public :
    aeEvent() {
        //创建时间管理对象
        //申请16个数据库
        //默认为０号数据库
        mask = 0 ;
    }
    ~aeEvent() {
    }
private :
    std::shared_ptr<TimerManager> tman ;
    //套接字对象
    aeSocket sock ;
    int connFd ;
    //设置数据库的编号
    int num ;
    //标志位
    int mask ;
    //用户缓冲区
    buffer buf ;
    //读回调函数   
    callBack readFunc ;
    //写回调函数
    callBack writeFunc ;
    //epoll事件
    std::shared_ptr<aeEpoll> aep ;
    int wakeFd ;
    int servFd ;
    epoll_event* ev ;
    int writeFd ;
    //在时间堆中的下标
    int timerIndex ;
    int blpopTimeout ;
public :
    static long times ;
public :
    int getIndex() { return timerIndex ; }
    void setIndex(int index) { timerIndex = index ; }
    int getBlpopTime() { return blpopTimeout ; }
    void setBlpopTime(int times) { blpopTimeout = times ; }
    int getWriteFd() { return writeFd ;}
    void setWriteFd(int fd) { this->writeFd = fd ;}
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
    shared_ptr<aeEpoll> getEp() { return aep ; }
    void setAep(shared_ptr<aeEpoll>aep) {
        this->aep = aep ;
    }
    void setTman(shared_ptr<TimerManager>tman) {
        this->tman = tman ;
    }
    shared_ptr<TimerManager> getTimerManager() {
        return tman ;
    }
} ;

