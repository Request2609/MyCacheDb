#include <iostream>
#include <sys/eventfd.h>
#include <unistd.h>

using namespace std ;

int main() {
    int efd = eventfd(0, EFD_CLOEXEC|EFD_NONBLOCK) ;
    while(1) {
        eventfd_t aa  ;
        cin >> aa ;
        eventfd_write(efd, aa) ;
        eventfd_t count ;
        eventfd_read(efd, &count) ;
        cout << count  << endl ;
    }
    close(efd) ;
    return 0;
}

