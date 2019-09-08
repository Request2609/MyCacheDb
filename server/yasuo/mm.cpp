#include <iostream>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include "aeEpoll.h"

using namespace std ;

int main() {
    int efd = eventfd(3, 0) ;
    if(efd < 0) {
        cout << __LINE__ << __FILE__ << endl ;
        return 1 ;
    }
    aeEpoll aep ;
    aep.epCreate(20) ;
    aep.add(efd, EPOLLIN) ;
    uint64_t i = 1 ;
    int ret = write(efd, &i, sizeof(i)) ;
    cout << "写入成功"<< endl ;
    if(ret < 0) {
        cout << "出错" << endl ;
        return -1 ;
    }
    while(1) {
        cout << "等待数据！"<< endl ;
        int ret =aep.wait() ;   
    }
    return 0;
}

