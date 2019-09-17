#include <iostream>
#include <memory>
#include <vector>
using namespace std ;

class A {
public :
    void print();
    virtual void say();
} ;
class B:public A {
public :
    B(){}
    void print() {
        printf("hello\n") ;
    }
    shared_ptr<A>b(new B) ;
    shared_ptr<A>b(new B) ;
} ;

int main() {
    A *b= new B;
    b->print() ;
    free(b) ;
}
