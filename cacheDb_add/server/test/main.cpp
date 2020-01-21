#include "easylogging++.h"
 
INITIALIZE_EASYLOGGINGPP
 
//如何设置配置文件
void init() {
 
    el::Configurations conf("logconf/log.conf") ;
    el::Loggers::reconfigureAllLoggers(conf) ;
}

int main(int argv, char* argc[]) {
    
    init() ;
    LOG(INFO)<<"MY first log config!" ;
    LOG(DEBUG) << "这里进行了DEBUG" << __LINE__ <<" " << __FILE__ ;
    LOG(ERROR) << "这里发生了错误 " << __LINE__ << "  " << __FILE__ ;
    return 0;
}
