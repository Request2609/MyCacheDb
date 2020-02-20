#pragma once

#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <memory>

#include "aeSocket.h"
#include "aeEpoll.h"


class signalSet {
public:
    signalSet() {}
    ~signalSet() {}
public :
    static int timeSlot ;
    static int pipeFd[2] ;
    static int efd ;
    static int wakeBlpopFd ;
public :
    static int createWakeBlpop() ;
    static void addSig(int sig) ;
    static void addWakeBlpopSig() ;
    static void wakeSigHandle() ;
    static void sigHandle(int sig) ;
    static int addToEpoll(std::shared_ptr<aeEpoll>ep) ;
    static int createPipeFd() ;
    static int createEventFd() ;
    static int setAlarm(int slot) ;
};

