#include "aofRecord.h"
#include "easylog.h"

INITIALIZE_EASYLOGGINGPP
const char* LOG_CONF= "../conf/aofLog.conf" ;
const int THREAD_NUM = 8 ;

aofRecord::aofRecord() {
    pool = std::make_shared<threadPool>(THREAD_NUM) ;
}

std::shared_ptr<aofRecord>aofRecord::rcd ;

aofRecord::~aofRecord() {
      el::Helpers::uninstallPreRollOutCallback();
    }
std::shared_ptr<aofRecord>aofRecord::getLogObject(){
    if(rcd == nullptr) {
        rcd = std::shared_ptr<aofRecord>(new aofRecord) ;
        rcd->init() ;
    }
    return rcd ;
}

void aofRecord::fullCall(const char* filename, std::size_t size) {
    rename(filename, "../logInfo/backuplog.log") ;
}   

void aofRecord::errorCall(const char* filename, std::size_t size) {
    rename(filename, "../logInfo/error_tmp.log") ;
}   


//设置滚动日志
int aofRecord::init() {
    el::Configurations conf(LOG_CONF);     
    el::Loggers::reconfigureAllLoggers(conf);
    el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck) ;
    /// 注册回调函数
    el::Helpers::installPreRollOutCallback(aofRecord::fullCall);
    return 1 ;
}

void aofRecord::record(const char* s) {
    const std::string ss = s ;
    //线程池就行异步持久化
    pool->commit(aofRecord::writeAofLog, ss) ;
}  

void aofRecord::log(const std::string& s) {
    LOG(ERROR)<<s ;
}


void aofRecord::writeAofLog(const std::string s) {
    LOG(INFO)<<s ;
}
