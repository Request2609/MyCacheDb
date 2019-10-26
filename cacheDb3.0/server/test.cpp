#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std ;
int main()
{
    int status ;
    int ret = fork() ;
    if(ret == 0) {
        cout << "进入子进程" << endl ;
    }
    else {
        waitpid(ret, &status, WNOHANG) ;
    }
    cout << "hello world!" << endl ;
    return 0;
}

