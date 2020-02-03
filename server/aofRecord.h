#pragma once
#include <iostream>
#include <memory>
#include "easylog.h"
#include "ThreadPool.h"

INITIALIZE_EASYLOGGINGPP
const char* LOG_CONF= "../conf/aofLog.conf" ;

class aofRecord {
public:
    ~aofRecord() {
      el::Helpers::uninstallPreRollOutCallback();
    }
    static std::shared_ptr<aofRecord>getLogObject() ;
    int init() ;
    static std::shared_ptr<aofRecord> rcd ;
    static void fullCall(const char* filename, std::size_t size) ;
    void record(const char*s) ;   
private:
    aofRecord() {}
};

