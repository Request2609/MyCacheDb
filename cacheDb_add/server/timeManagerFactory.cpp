#include "timeManagerFactory.h"

shared_ptr<TimerManager>timeManagerFactory ::clockEvent ;
shared_ptr<TimerManager> timeManagerFactory::wakeBlpop  ;

//使用单例和工厂模式的结合
//1 是不活跃连接的定时器
//2 是唤醒blpop事件的是定时器
shared_ptr<TimerManager> timeManagerFactory:: getManager(int tip) { 
    if(tip == 1) {
        if(clockEvent == nullptr) {
            clockEvent = make_shared<TimerManager>();
        }
        return clockEvent ;
    }
    else {
        if(wakeBlpop == nullptr) {
            wakeBlpop = make_shared<TimerManager>() ;
        }
        return wakeBlpop ;
    }
}
