#include "signalSet.h"

int signalSet :: timeSlot = 10000 ;
int signalSet :: pipeFd[2] ;
int signalSet :: efd ;
void signalSet :: addWakeBlpopSig() {
    
}

int signalSet :: createEventFd() {
    efd = aeSocket :: createEventFd() ;
    if(efd < 0) {
        return -1 ;
    }
    return efd ;
}

int signalSet :: createWakeBlpop() {
    wakeBlpopFd = efd ;
    return wakeBlpopFd ;
}

int signalSet :: createPipeFd() {
    
    int ret = aeSocket::createSocketPair() ;  
    pipeFd[0] = aeSocket :: getReadFd() ;
    pipeFd[1] = aeSocket :: getWriteFd() ;
    return ret ;
}

int signalSet :: setAlarm(int slot) {
    //设置时钟信号
    alarm(slot) ;      
    return 1 ;
}

