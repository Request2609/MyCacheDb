#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#define _GUN_SOURCE
#define BUFFER_SIZE 8192
int  main()
{ 
    int fd = open("testfile", O_CREAT|O_RDWR|O_DIRECT) ;
    char* realbuff = (char*)malloc(BUFFER_SIZE+pagesize) ;
    write()
    free(realbuff) ;
    std::cout << "Hello world" << std::endl;
    return 0;
}

