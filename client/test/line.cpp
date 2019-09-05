#include <iostream>
#include <readline/readline.h>
#include <readline/history.h> 
using namespace std ;
int main() {
    while(true) {
        char *p = readline("myRedis >> ") ;
        cout << p << endl ;
        add_history(p) ;
    }
    return 0;
}

