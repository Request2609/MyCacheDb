#include <iostream>
#include <sys/eventfd.h>
#include <sys/unistd.h>
#include <string.h>
using namespace std ;

int main() {
    
    int efd = eventfd(0, EFD_CLOEXEC) ;
    if(-1 == efd) {
        cout << "创建失败！" << endl ;
        return -1 ;
    }
    int ret = fork() ;
    if(ret == 0) {
        for(uint64_t i=0; i<5; i++) {
            cout << "发送消息：" << i << endl ;
            int r = write(efd, &i, sizeof(uint64_t)) ;   
            if(r < 0) {
                cout << "发送出错！" << strerror(errno)<< endl ;
                return -1 ;
            }
        }
        exit(EXIT_SUCCESS) ;
    }
    else {
        sleep(2) ;
        uint64_t ret ;
        int t = read(efd, &ret, sizeof(uint64_t)) ;
        if(t < 0) {
            cout << "接收到消息！" << endl ;
            return -1 ;
        }
        cout <<"接收到消息："<< ret << endl ;
        exit(1) ;
    }
    return 0;
}

