#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

using namespace std ;


#define handle_error(msg) do{perror(msg);exit(EXIT_FAILURE);} while(0)

int main(int argc, char *argv[])
{
        int fd;
        off_t length;
        char *addr;
        char *inserted = "## inserted ##"; // this str will be inserted to the file
        int pos = 5; // the position to insert
        fd = open("abc.txt", O_RDWR | O_CREAT, 0644);
        if(fd == -1)
        {
                handle_error("open file error");
        }
        length = lseek(fd, 1, SEEK_END);
        write(fd, "\0", 1);
        addr = (char *)mmap(NULL, length + strlen(inserted), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        memcpy(addr + pos + strlen(inserted), addr + pos, length - pos);
        memcpy(addr + pos, inserted, strlen(inserted));
        //printf("addr: %s", addr);
        close(fd);
        munmap((void *)addr, length);
}
