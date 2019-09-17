#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std ;

int main() {
    int fd = open("hello", O_RDWR|O_APPEND) ;
    if(fd < 0) {
        return 1;
    }
    struct stat st ;
    fstat(fd, &st) ;
    cout << "文件大小！"<< st.st_size << endl ;
    char* addr = (char*)mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, 0) ;
    close(fd) ;
    cout << addr  << endl ;
    munmap(addr, st.st_size) ;
}
