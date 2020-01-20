#pragma once
#include <mutex>
#include <memory>
#include <iostream>
using namespace std ;

class checkOperation {
public:
    ~checkOperation() {}
    static shared_ptr<checkOperation> getCheckObject() ;
    static bool checkWriteTimes() ;
private:
    checkOperation(int times) :writeTimes(times){
    }
    shared_ptr<checkOperation> chk ;
    int writeTimes ;
};

