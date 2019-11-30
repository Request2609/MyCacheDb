#include "listWaitQueue.h" 

queue<shared_ptr<aeEvent>> listWaitQueue::waitQ ;


void listWaitQueue:: add(shared_ptr<aeEvent> tmp) {
    waitQ.push(tmp) ;   
}

shared_ptr<aeEvent> listWaitQueue:: popEvent() {
    shared_ptr<aeEvent>tmp = waitQ.front() ;
    waitQ.pop() ;
    return tmp ;
}


