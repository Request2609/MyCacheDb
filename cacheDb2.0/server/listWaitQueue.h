#pragma once
#include <queue>
#include "aeEvent.h" 

class listWaitQueue {
public:
    listWaitQueue() {}
    ~listWaitQueue() {
    }
    static void add(shared_ptr<aeEvent> tmp) ;
    static shared_ptr<aeEvent> popEvent() ;
private:
    static queue<shared_ptr<aeEvent>>waitQ ;
};

