#include <iostream>
#include <memory>
using namespace std ;

class A {
public :
    ~A() {}
    static void record() {
        shared_ptr<A>a = getOb() ;
        a->print() ;
    }
    static shared_ptr<A>a ;
private:
    static shared_ptr<A> getOb() {
        if(a == nullptr) {
            a = shared_ptr<A>(new A) ;
        }
        return a ;
    }
    A(){}
    void print() {
        printf("hello world!") ;
    }
} ;


shared_ptr<A>A::a = nullptr;

int main() {
    A::getOb()->print() ;
    return 0;
}

