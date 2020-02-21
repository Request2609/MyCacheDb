#include "syncQueue.h"

std::shared_ptr<syncQueue> syncQueue::que ;
std::shared_ptr<syncQueue> syncQueue::getQueue() {
    if(que == nullptr) {
        que = std::shared_ptr<syncQueue>(new syncQueue) ;
    }
    return que ;
}

int syncQueue::reqEmpty() { 
    return req.size() ;
}

long syncQueue::addToQueue() {
    struct timeval start;
    gettimeofday( &start, NULL );
    long ii = start.tv_sec+start.tv_usec;
    req.push(ii) ;
    return ii ;
}

void syncQueue::getResponse() {
    auto s = req.front() ;
    if(getResponse(s) == 1) {
        req.pop() ;
    }
}

void syncQueue::addToResponse(long num, const std::string s) {
    std::lock_guard<std::mutex>lk(mute) ;
    response.insert({num, s}) ;
}

int syncQueue::getResponse(long num) {
    auto ret = response.find(num) ;
    if(ret == response.end()) {
        std::cout << "还没收到命令回复"<< std::endl ;
        return -1 ;
    }
    else {
        response.erase(ret) ;
        return 1 ;
    }
}
