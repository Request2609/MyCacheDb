#include <iostream>

using namespace std ;

class lll {
public :
    int a ;
    int b ;
} ;

class bbb {
public :
    static lll get() {
        lll l ;
        l.a =10 ;
        l.b = 11 ;
        return l ;
    }
} ;
int main() {
    lll l ;
    l = bbb::get() ;
    cout << l.a << "       " << l.b << endl ;
    return 0;
}

