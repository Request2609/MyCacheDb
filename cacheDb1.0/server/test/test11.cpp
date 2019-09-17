#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
using namespace std ;
int main()
{
    int fd = open("hello",O_RDWR) ;
    if(fd < 0) {
        cout << __FILE__ << "   " << __LINE__ << endl ;
        return 1;
    }
    char name[1024] ;
    bzero(name, sizeof(name)) ;
    if(pread(fd,name, 5, 10) < 0) {
        cout << "fsafdf" << endl ;
        return 1;
    }
    cout << name << endl ;
    close(fd) ;
    fd = open("hello", O_CREAT|O_WRONLY|O_APPEND, 0766) ;
    if(fd < 0) {
        cout << "创建出错！" <<"    " << strerror(errno)<< endl ;
        return 1;
    }
    pwrite(fd, name, 5, 50) ;
    close(fd) ;
    return 0;
}

