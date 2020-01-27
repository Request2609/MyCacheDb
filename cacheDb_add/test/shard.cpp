#include <iostream>
#include <memory>
using namespace std ;

class Test {
public :
    Test() {
        cout << "调用构造函数" << endl ;
    }
    ~Test(){}
    void print() {
        cout << "hello world!    " << a<<endl ;
    }
    int a ;
} ;
int main() {
    shared_ptr<Test>ls = make_shared<Test>() ;
    ls->a = 10 ;
    ls->print() ;

    shared_ptr<Test>ss = make_shared<Test>(*ls) ;
    ss->a = 11 ;
    ss->print() ;
    ls->print() ;
    return 0;
}

