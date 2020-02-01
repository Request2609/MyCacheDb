#include "timer.h"
//获取当前的时间
long timer :: getCurTime() {
#ifdef _MSC_VER
	_timeb timebuffer;
	_ftime(&timebuffer);
	unsigned long long ret = timebuffer.time;
	ret = ret * 1000 + timebuffer.millitm;
	return ret;
#else
	timeval tv;
	::gettimeofday(&tv, 0);
	unsigned long long ret = tv.tv_sec;
	return ret * 1000 + tv.tv_usec / 1000;
#endif
}
