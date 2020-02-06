#include "aofRecord.h"
#include "easylog.h"

INITIALIZE_EASYLOGGINGPP
const char* LOG_CONF= "../conf/aofLog.conf" ;


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
    std::cout << "记录日志:" << s << std::endl ;
    LOG(INFO) << s ;
}  

