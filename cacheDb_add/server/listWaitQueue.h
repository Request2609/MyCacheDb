#pragma once
#include <queue>
#include "aeEvent.h" 

class aeEvent ;
class listWaitQueue {
public:
    listWaitQueue() {}
    ~listWaitQueue() {
    }
    static shared_ptr<aeEvent> get() ;
    static void add(shared_ptr<aeEvent> tmp) ;
    static shared_ptr<aeEvent> popEvent() ;
    static void del(int fd) ;
    static bool isEmpty() { return waitQ.size() ; }
private:
    static vector<shared_ptr<aeEvent>>waitQ ;
};

