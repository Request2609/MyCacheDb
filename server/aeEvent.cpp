#include "aeEvent.h"
#include "serializeParse.h"

int aeEvent :: processRead() {
	if(mask == event::timeout) {
		uint64_t ret = 0 ;
		int res = read(connFd, &ret, sizeof(ret)) ;
		if(ret < 0) {
			cout << __FILE__ << "      " << __LINE__ << endl ;
			return 1 ;
		}
		res = readFunc(std::shared_from_this()) ;
		return ret ;
	}
    //读数据
    buf.init() ;
    int ret = read(connFd, buf.getBuf(), SIZE) ;
    if(ret < 0) {
        std::string s = "         " +std::to_string(__LINE__) +__FILE__ ;
        aofRecord::log(s) ;
        //事件
        return -1 ;
    }
    if(ret == 0) {
		//一个客户端断开连接
		return 0 ;
	}
	//根据返回值，将数据读到buf
	//将数据返回给事件循环/*
	//调用相应的回调处理可读事件
	ret = readFunc(std::shared_from_this()) ;
	return ret ;
	//先存入缓冲区
	//判断收到结束标志调用回调函数  <接口>预留
}
int aeEvent :: processWrite() {
	int ret = 0 ;
	return ret ;
}
