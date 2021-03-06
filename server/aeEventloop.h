#pragma once
#include <functional>
#include <vector>
#include <map>
#include <algorithm>
#include <memory> 
#include <sys/eventfd.h>
#include <fstream>
#include "signalSet.h"
#include "timerHeap.h" 
#include "aeEpoll.h"
#include "aeEvent.h"
#include "msg.pb.h"


const int timeSlot = 10000;

enum {
    READ= EPOLLIN, 
    WRITE= EPOLLOUT,
} ;

class TimerManager ;
class aeEvent ;
//创建事件循环
class aeEventloop {
    typedef std::function<int(std::shared_ptr<aeEvent>)> callBack ;
    
public :
    //客户端的读写回调
    callBack  readCall ;
    callBack writeCall ;
    callBack timerCall ;
    //监听事件的集合
    std::vector<int>listenFd ;
    //epoll事件
    std::shared_ptr<aeEpoll> aep ;
    //最大文件描述符的值，感觉在epoll中也没什么用
    int maxFd ;
    //文件描述符的个数
    int setSize ; 
    //下一个时间事件ID标识
    long long timeEventNextId ;
    //设置事件
    std::map<int, std::shared_ptr<aeEvent>> eventData ;
    //停止标志
    int stop ;
    //处理底层API的数据
    void* apiData ;
    //触发的事件列表
    std::vector<epoll_event>fireList ;
    //db服务器的保存的条件
    //像定时条件
    std::map<std::string, long>param ;
    //记录距离上一次save，服务器修改了多少次数据库状态
    long dirty ;
    //现在距离上一次执行save命令的时间
    long lastSave ;
    //定时事件
    int timeFd ;
    std::shared_ptr<TimerManager> tman ;
    std::shared_ptr<TimerManager> wakeblpop ;
    int evfd ;
    int signalFd ;
    int saveFd ;
public :
    static int canSave ;
    static int wakeBlpopFd  ;
    static int efd ;
    static int kickClient(std::map<int, std::shared_ptr<aeEvent>>&eventData, 
                          int kickFd, std::shared_ptr<aeEpoll>&aep) ;
    static int sendResponse() ;
public :
    aeEventloop() ; 
    ~aeEventloop() ;
public :
    static int notifyToSave(int fd) ;
public :
    void initDataInfo() ;
    std::shared_ptr<aeEpoll> getEp() { 
        return aep ; 
    }
    int addTimerEvent() ;
    //时间事件回调
    void setReadCallBack(callBack readCb) { 
        timerCall = readCall = readCb ; 
    }
    int init() ;
    int acceptNewConnect(int fd) ;
    void setCallBack(callBack readCb, callBack writeCb) ;
    int addServerEvent(std::string addr, std::string port) ;
    int start() ;
    int aeProcessEvent(int fd) ;
} ;

int getIpPort(std::string& ip, std::string& port) ;
