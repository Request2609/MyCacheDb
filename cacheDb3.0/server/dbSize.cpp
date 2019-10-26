#include "dbSize.h"

unsigned long long dbSize :: size = 0 ;

//记录值增加
void  dbSize :: addSize(unsigned long& s) {
    size += s ;
}

//按照传入的参数减少记录值
void dbSize :: releaseSize(unsigned long& s) {
    size -= s ;
}

//获取记录值大小，以字节为单位
unsigned long long  dbSize :: getSize() {
    return size ;
}

//获取兆字节
int dbSize :: getMB() {
    return size/1024*1.0 ;
}

//获取GB字节
int dbSize :: getGB() {
    return size/(1024*1024)*1.0 ;
}
