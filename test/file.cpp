#include <iostream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
using namespace std ;

int main() {
    int fd = open("test", O_CREAT|O_RDWR|O_SYNC, 0666) ;
    if(fd < 0) {
        cout << "打开错误" << endl ;
        return 1 ;
    }
    char buf[4096] ;
    bzero(buf, sizeof(buf)) ;
    strcpy(buf, "hello world!") ;
    cout << fd << endl; 
    int ret = write(fd,  buf, sizeof(buf)) ;
    if(ret < 0) {
        cout << __LINE__ << "   " << __FILE__ << "   " << strerror(errno)<< endl ;
        return 0 ;
    }
    close(fd) ;
    return 0 ;
}

