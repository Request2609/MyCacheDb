#include <iostream>
#include <memory>
#include <thread>
using namespace std ;

class A {
public :
    A() {}
    void print() {
        cout << "Hello World!" << endl ;
    }
} ;

void func(const shared_ptr<A>a) {
    a->print() ;
}
int main() {
    shared_ptr<A>a = make_shared<A>();
    thread t(func, a) ;
    t.join() ;
    return 0;
}

