#include "logErr.h"

void logErr::init() {
    el::Configurations conf("../logconf/log.conf") ;
    el::Loggers::reconfigureAllLoggers(conf) ;
}

void logErr::record(std::string s) {
    LOG(ERROR)<<s ;
}
