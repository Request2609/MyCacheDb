#include "listWaitQueue.h" 

vector<shared_ptr<aeEvent>> listWaitQueue::waitQ ; 


shared_ptr<aeEvent> listWaitQueue:: get() {
    for(auto ss =waitQ.begin();  ss!= waitQ.end(); ss++) {
        shared_ptr<aeEvent> tmp = *ss ;
        waitQ.erase(ss) ;
        break ;
    }
}

void listWaitQueue:: add(shared_ptr<aeEvent> tmp) {
    waitQ.push_back(tmp) ;   
}

shared_ptr<aeEvent> listWaitQueue:: popEvent() {
    shared_ptr<aeEvent>tmp = waitQ.front() ;
    waitQ.pop_back() ;
    return tmp ;
}

void listWaitQueue :: del(int fd) {
    for(auto q = waitQ.begin(); q!=waitQ.end(); q++) {
        if((*q)->getConnFd() == fd) {
            waitQ.erase(q) ;
            break ;
        }
    }  
}

