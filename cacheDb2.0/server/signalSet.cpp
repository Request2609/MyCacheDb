#include "signalSet.h"

int signalSet :: timeSlot = 1000 ;
//等待被触发的时钟信号
void signalSet :: addSig(int sig) {
    struct sigaction sa ;
    memset(&sa, '\0', sizeof(sa)) ;
    sa.sa_handler = sigHandle ;
    sa.sa_flags |= SA_RESTART ;
    sigfillset(&sa.sa_mask) ;
    assert(sigaction(sig, &sa, NULL) != -1) ;
}


int signalSet :: createEventFd() {
    efd = aeSocket::createEventFd() ;
    return efd ;
}

int signalSet :: setAlarm() {
    //设置时钟信号
    alarm(timeSlot) ;      
    return 1 ;
}

//向eventfd写数据，触发定时器任务
void signalSet :: sigHandle(int sig) {
    eventfd_t  data = 1;
    write(efd, &data, sizeof(data)) ;
}

int signalSet :: createPipePair() {
    int ret = aeSocket::createSocketPair() ;
    if(ret < 0) {
        return -1 ;
    }
    pipeFd[0] = aeSocket :: getReadFd() ;
    pipeFd[1] = aeSocket :: getWriteFd() ;
    return 1 ;
}


