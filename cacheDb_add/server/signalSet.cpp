#include "signalSet.h"

int signalSet :: timeSlot = 10000 ;
int signalSet :: pipeFd[2] ;
int signalSet :: efd ;
int signalSet:: wakeBlpopFd ;
//int signalSet :: efd ;
//等待被触发的时钟信号
void signalSet :: addSig(int sig) {
    struct sigaction sa ;
    memset(&sa, '\0', sizeof(sa)) ;
    sa.sa_handler = sigHandle ;
    sa.sa_flags |= SA_RESTART ;
    sigfillset(&sa.sa_mask) ;
    assert(sigaction(sig, &sa, NULL) != -1) ;
}

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

//向eventfd写数据，触发定时器任务
void signalSet :: sigHandle(int sig) {
    int save_errno = errno ;
    eventfd_t  data = 10 ;
    write(efd, &data, sizeof(data)) ;
    errno = save_errno ;
}

