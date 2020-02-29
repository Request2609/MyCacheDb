#pragma once
#include <iostream>
#include <vector>
#include <sys/epoll.h> 
#include <string.h>
#include <unistd.h>

class aeEpoll
{
public:
    aeEpoll() {
        nfds = 200 ;
        epFd = -1 ;
    }
    ~aeEpoll() {close(epFd) ;}
public : 
    int add(int fd, int events) ;
    int epCreate(int size) ;   
    int modify(int fd, int event) ;
    int del(int fd) ;
    int wait(std::vector<epoll_event>&ls) ;
    int getEpFd() { return epFd ; }
private :
    int epFd ;
    //初始化最大wait到的活动文件描述符数量
    int nfds  ;  
    std::vector<struct epoll_event>eventFds ;
};

