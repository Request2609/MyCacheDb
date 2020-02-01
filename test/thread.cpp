#include <iostream>
#include <thread>
#include <memory>
using namespace std ;
class Test{
public :
    Test() {

    }
    ~Test() {

    }
    void print() {
        cout << "jhello" << endl ;
    }
} ;
void func(shared_ptr<Test>tt) {
    tt->print() ;
}
int main()
{
    shared_ptr<Test> ptr = make_shared<Test>() ;
    thread t1(ref(ptr)) ;
    t1.join() ;
    return 0;
}

