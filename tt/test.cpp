#include "easylog.h"
 
#include <iostream>

using namespace std ;
INITIALIZE_EASYLOGGINGPP
 
static unsigned int idx;
 
void rolloutHandler(const char* filename, std::size_t size) 
{
    cout << filename<< endl ;
	/// 备份日志
	std::stringstream ss;
	system(ss.str().c_str());
}
 
int main(int, char**)
{
	idx = 0;
    el::Configurations conf("conf/aofLog.conf") ;
	el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
    el::Loggers::reconfigureAllLoggers(conf) ;
	/// 注册回调函数
	el::Helpers::installPreRollOutCallback(rolloutHandler);
 
	for (int i = 0; i < 1; ++i)
	{
		LOG(INFO) << "Test";
	}
 
	/// 注销回调函数
	el::Helpers::uninstallPreRollOutCallback();
	return 0;
}
