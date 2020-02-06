#pragma once
#include <iostream>
#include <memory>
#include "ThreadPool.h"
class aofRecord {
public:
    ~aofRecord() ;
    static std::shared_ptr<aofRecord>getLogObject() ;
    int init() ;
    static std::shared_ptr<aofRecord> rcd ;
    static void fullCall(const char* filename, std::size_t size) ;
    void record(const char*s) ;   
private:
    aofRecord() {}
};

