#include <iostream>
#include <memory>

using namespace std ;

class test {
public :
    static shared_ptr<test> getOb() {
        if(tt == nullptr) {
            tt = shared_ptr<test>(new test(1)) ;
        } 
        return tt ;
    }
    void print() {
        cout << a << endl ;
    }
private :
    test(int s):a(s) {}
    int a ;
    static shared_ptr<test> tt ;
} ;
shared_ptr<test>test::tt = nullptr ;
int main()
{
    shared_ptr<test> tt = test::getOb() ;
    tt->print() ;
    return 0;
}

