#include "aofRecord.h"

std::shared_ptr<aofRecord>aofRecord::rcd ;
std::shared_ptr<aofRecord>aofRecord::getLogObject(){
    if(rcd == nullptr) {
        rcd = std::shared_ptr<aofRecord>(new aofRecord) ;
        rcd->init() ;
    }
    return rcd ;
}

void aofRecord::fullCall(const char* filename, std::size_t size) {
    rename(filename, "backuplog.log") ;
}   

//设置滚动日志
int aofRecord::init() {
    el::Configurations conf("../conf/aofLog.conf");     
    el::Loggers::reconfigureLogger("default", conf);
    el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck) ;
    el::Loggers::reconfigureAllLoggers(conf);
    /// 注册回调函数
    el::Helpers::installPreRollOutCallback(aofRecord::fullCall);
    return 1 ;
}

void aofRecord::record(const char* s) {
    const std::string ss = s ;
    LOG(INFO) << s ;
}  

