#include <iostream>
#include <memory>
using namespace std ;

class A {
public :
    int a ;
} ;
int shared(shared_ptr<A>ss) {
    ss->a = 9 ;
    return 1 ;
}

int main() {
    shared_ptr<A>aa(new A) ;
    aa->a =1 ;
    shared(aa) ;
    cout << aa->a << endl ;
    return 0;
}

