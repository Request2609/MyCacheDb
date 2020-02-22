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
    static void errorCall(const char* file, std::size_t size) ;
    void record(const char*s) ;   
    static void log(const std::string& s) ;
    static void writeAofLog(const std::string s) ; 
private:
    aofRecord() ;
    std::shared_ptr<threadPool> pool ;
};

