#include <thread>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include "easylogging++.h"
 
INITIALIZE_EASYLOGGINGPP
const int times= 10000 ;
//如何设置配置文件
void init() {
    el::Configurations conf("logconf/log.conf") ;
    el::Loggers::reconfigureAllLoggers(conf) ;
}

void func(int fd) {
    char buf[1024] ;
    bzero(buf, sizeof(buf)) ;
    for(int i=0; i<times; i++) {
        sprintf(buf, "第%d条信息\n", i) ;
        write(fd, buf, sizeof(buf)) ;
    }
}

int main(int argv, char*argc[]) {
    
    //init() ;
    int fd = open("logrecord", O_WRONLY) ;
    std::thread t1(func, fd) ;
    std::thread t2(func, fd) ;
    std::thread  t3(func, fd) ;

    t1.join() ;
    t2.join() ;
    t3.join() ;
    close(fd) ;
    std::ifstream out ;
    out.open("logrecord", std::ios::out|std::ios::in) ;
    if(out.fail()) {
        return 1;
    }
    else {
        std::string s ;
        while(!out.eof()) {
            out >> s ;
            std::cout << s<< std::endl ;
        }
    }
    out.close() ;
    return 0;
}
