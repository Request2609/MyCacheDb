#include <iostream>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std ;
int main()
{
    int fd = open("hello", O_RDWR|O_APPEND) ;
    const char buf1[] ="hello world\n";
    const char buf2[] ="hello china" ;

    write(fd, buf1, sizeof(buf1)) ;
    write(fd, buf2, sizeof(buf1)) ;
    close(fd) ;
    return 0;
}

