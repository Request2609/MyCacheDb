#include "saveTimerHandle.h"
std::shared_ptr<saveTimerHandle> saveTimerHandle::sth ;
std::map<int, int>saveTimerHandle::counts ;

std::shared_ptr<saveTimerHandle> 
saveTimerHandle::getSaveTimerObject() {
    if(sth == nullptr){
        sth = std::shared_ptr<saveTimerHandle>(new saveTimerHandle) ;
    }
    return sth ;
}

void saveTimerHandle::getStrategy() {
    getInfo(strategy)  ;
    if(strategy.empty()) {
        return ;
    }
    for(auto s : strategy) {
        std::count[s->first] = 0;
    }
}

void saveTimerHandle::countModify() {
    if(sth->start == 0) return ;
    for(auto s : counts) {
        s.second++ ;
    }
}

int saveTimerHandle::setEpoll(shared_ptr<aeEpoll>aep) {
    if(sth == nullptr) {
        getSaveTimerObject() ;
    }
    sth->aep = aep ;
}

void saveTimerHandle::startDetect() {
    getStrategy() ;
    if(strategy.empty()||aep == nullptr) {
        return  ;
    }

    for(auto s : strategy) {
        int sec = s.first ;
        int fd = setTimer(sec) ;   
        if(fd < 0) {
            return ;
        }
        aep->add(fd, EPOLLIN);
    }
    start = 1 ;
}

int saveTimerHandle::setTimer(int sec) {
   struct timespec now ;
   if(clock_gettime(CLOCK_REALTIME, &now)) {
       cout << __FILE__ << "    "<< __LINE__ << endl ;
        return -1 ;
   }
    struct itimerspec itime ;
    //设置初始时间
    itime.it_value.tv_sec = now.tv_sec+sec ;
    itime.it_value.tv_nsec = now.tv_nsec ; 

    //设置间隔时间
    itime.it_interval.tv_sec = sec ; //seconds
    itime.it_interval.tv_nsec = 0 ; //(ns)//精度太高>
    int fd = timerfd_create(CLOCK_REALTIME, 0) ;
    if(fd < 0) {
        cout << __FILE__ << "      " << __LINE__ << endl ;
        return -1 ;
    }
    if(timerfd_settime(fd,TFD_TIMER_ABSTIME, &itime, NULL) == -1) {
        cout << __FILE__ <<"       " << __LINE__  <<endl ;
        return -1 ;
    }
    return fd ;
}

int saveTimerHandle::isSaveHandle(int fd) {
    auto res = strategy.find(fd) ;
    if(start == 0 || res == strategy.end()) {
        return -1 ;
    }
    else {
        uint64_t num ;
        read(fd, &num, sizeof(num)) ;
        if(res->second > counts[res->first] && aeEventloop::canSave) {
            cmdSet::backUp() ;
            counts[res->first] = 0 ;
        }
    }
    return 1 ;
}

