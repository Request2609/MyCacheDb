#include <iostream>
#include <functional>
using namespace std ;

class AA {
typedef function<void ()> callBack ;
public :
AA(callBack cb) {
    call = cb ;
}
    ~AA() {}
    void callRun() {
        call() ;
    }
private :
    callBack call ;
} ;

void print(){
    cout << "hello world!" << endl ;
}
int main() {
    AA a(print) ;
    a.callRun() ;
    return 0;
}

