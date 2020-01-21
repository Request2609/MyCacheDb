#include <iostream>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
using namespace std ;
int main()
{
    int fd = open("readfile_", O_RDWR, 0666) ;
    if(fd < 0) {
        cout << "错误"<< endl ;
        return 0 ;
    }
    else {
        const char* buf = "hello world!" ;
        write(fd , buf,strlen(buf)) ;
    }
    close(fd) ;
    return 0;
}

