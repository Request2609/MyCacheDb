#pragma once

#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <memory>

#include "aeSocket.h"
#include "aeEpoll.h"

using namespace std ;

class signalSet {
public:
    signalSet() {}
    ~signalSet() {}
public :
    static int timeSlot ;
    static int pipeFd[2] ;
    static int efd ;
public :
    static void addSig(int sig) ;
    static void sigHandle(int sig) ;
    static int addToEpoll(shared_ptr<aeEpoll>ep) ;
    static int createPipeFd() ;
    static int createEventFd() ;
    static int setAlarm(int slot) ;
};

