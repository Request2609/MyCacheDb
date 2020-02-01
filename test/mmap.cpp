#include <iostream>
#include <sys/mman.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <sys/stat.h>

using namespace std ;

const char* file = "hello" ;

void setFlag() {
    int fd = open(file, O_RDWR|O_CREAT) ;
    struct stat st ;
    if(fstat(fd, &st) < 0) {
        cout << __FILE__ << "     " << __LINE__ << endl ; 
        return ;
    }
    char* flag = (char*)mmap(NULL, st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0) ;
    close(fd) ;
    flag[0] = '1' ;
    //释放映射区
    munmap((void*)flag, st.st_size) ;
}


int main() {
    setFlag() ;  
    return 0;
}

