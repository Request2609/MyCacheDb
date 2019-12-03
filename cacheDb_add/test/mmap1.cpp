#include <iostream>
#include <sys/mman.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

const char* file = "hello" ;

using namespace std ;

char getFlag() {
    char a = '-' ;
    int fd = open(file, O_RDWR|O_CREAT) ;
    if(fd < 0) {
        cout << __FILE__ << "     " << __LINE__ << endl ;
        return a;
    }
    struct stat st ;
    if(fstat(fd, &st) < 0) {
        cout << __FILE__ << "     " << __LINE__ << endl ; 
        return a ;
    }
    
    char* flag = (char*)mmap(NULL, st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0) ;
    close(fd) ;
    a =  flag[0] ;
    flag[0] = '0' ;
    munmap((void*)flag, st.st_size) ;
    return a ;
}

int main() {
    char c = getFlag() ;
    cout << c << endl ;
    return 0;
}

