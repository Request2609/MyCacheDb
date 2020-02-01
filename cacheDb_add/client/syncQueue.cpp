#include "syncQueue.h"

shared_ptr<syncQueue> syncQueue::que ;
shared_ptr<syncQueue> syncQueue::getQueue() {
    if(que == nullptr) {
        que = shared_ptr<syncQueue>(new syncQueue) ;
    }
    return que ;
}

long syncQueue::addToQueue() {
    time_t t ;
    t = time(NULL) ;
    long ii = time(&t) ;
    req.push(ii) ;
    return ii ;
}

void syncQueue::getResponse() {
    auto s = req.front() ;
    if(getResponse(s) == 1) {
        req.pop() ;
    }
}

void syncQueue::addToResponse(long num, const string s) {
    lock_guard<mutex>lk(mute) ;
    response.insert({num, s}) ;
}

int syncQueue::getResponse(long num) {
    auto ret = response.find(num) ;
    if(ret == response.end()) {
        cout << "还没收到命令回复"<< endl ;
        return -1 ;
    }
    else {
        cout << ret->second << endl ;
        response.erase(ret) ;
        return 1 ;
    }
}
