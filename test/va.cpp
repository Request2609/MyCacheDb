#include <iostream>
#include <stdarg.h>
#include <strings.h>
#include <string.h>
using namespace std ;

void demo(const char* msg, ...) {
    va_list argp ;
    int argno = 0 ;
    char* para ;
    va_start(argp, msg) ;
    while(1) {
        para = va_arg(argp, char*) ;
        if(strcmp(para, "") == 0) {
            break ;
        }
        cout << "parameter #" << argno << " is  " << para << endl ;
        argno++ ;
    }
}
int main()
{   
    const char * cp = "hello" ;
    demo(cp, "wo", "llall", "lalal") ;
    return 0;
}

