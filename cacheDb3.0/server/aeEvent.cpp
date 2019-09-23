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
        res = readFunc(shared_from_this()) ;
        return ret ;
    }
    //读取数据
    char buff[SIZE] ;
    //读数据
    int ret = read(connFd, buff, sizeof(buff)) ;
    if(ret < 0) {
        cout << __FILE__  << "      " << connFd << "      "  << __LINE__ << "                " << strerror(errno) << endl ;
        return -1 ;
    }
    if(ret == 0) {
        //一个客户端断开连接
        cout << "one user disconnect!" << endl ;
        return 0 ;
    } 
    //根据返回值，将数据读到buf
    for(int i=0; i<ret; i++) {
        buf.append(buff[i]) ;
    }

    //收到数据
//    string* aa = buf.getBuf() ;
    //将数据返回给事件循环/*
    //调用相应的回调处理可读事件
    ret = readFunc(shared_from_this()) ;
    return ret ;
    //先存入缓冲区
    //判断收到结束标志调用回调函数  <接口>预留  
}


int aeEvent :: processWrite() {
    int ret = 0 ; 
 //   int ret =  writeFunc() ; 
    return ret ;
}
