#pragma once
#include <map>
#include <memory>
#include <unistd.h>
#include <time.h>
#include <sys/timerfd.h>
#include "readWrite.h"
#include "aeEpoll.h"
#include "cmdSet.h"
#include "aeEventloop.h" 

class cmdSet ;
class aeEventloop ;

class saveTimerHandle {
public:
    ~saveTimerHandle() {
        for(auto s : strategy) {
            close(s.first) ;
        }
    }
    static std::shared_ptr<saveTimerHandle> getSaveTimerObject() ;
    static void countModify() ;
    int setEpoll(shared_ptr<aeEpoll>aep) ;
    void startDetect() ;
    int isSaveHandle(int fd) ;
private:
    int start ;
    saveTimerHandle():start(0) {}
    void getStrategy() ;
    int setTimer(int sec) ;
    static std::map<int, int>counts ;
    std::map<int, int> strategy ;
    static int modifyCount ;
    std::shared_ptr<aeEpoll>  aep ;
    static std::shared_ptr<saveTimerHandle> sth ;
};

