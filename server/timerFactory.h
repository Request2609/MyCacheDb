#pragma once
#include <memory>
#include <iostream>
using namespace std ;
#include "timerHeap.h"

class TimerManager ;

class timeManagerFactory {
public:
    timeManagerFactory() {}
    ~timeManagerFactory() {}
public :
    static shared_ptr<TimerManager> clockEvent ;
    static shared_ptr<TimerManager> wakeBlpop ;
    static shared_ptr<TimerManager> getManager(int tip) ;
};
