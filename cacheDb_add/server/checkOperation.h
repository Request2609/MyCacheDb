#pragma once
#include <mutex>
#include <memory>
#include <map>
#include <iostream>
#include <fstream>
#include "aeSocket.h"

using namespace std ;

class checkOperation {
public:
    ~checkOperation() {}
    static shared_ptr<checkOperation> getCheckObject() ;
    static bool checkWriteTimes() ;
    void getSaveRule() ;
private:
    checkOperation(){
    }
    int eventFd ;
    shared_ptr<checkOperation> chk ;
    map<int, int>modifyMap ;
    int writeTimes ;
};


