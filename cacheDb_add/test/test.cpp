#include <iostream>
#include <memory>
using namespace std ;

class A {
public :
    A(int s) :a(s){}
    ~A() {}
public :
    int a ;
    void print() {
        cout << a << endl ;
    }
} ;

void swap(shared_ptr<A>&aa) {
    cout << "swap:" << aa->a << endl ;
    aa->a = 19 ;
}
int main()
{
    shared_ptr<A>aa = make_shared<A>(10) ;
    aa->print() ;
    swap(aa) ;
    aa->print() ;
    return 0;
}

