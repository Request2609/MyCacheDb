#include <iostream>
#include <map>
#include <stdio.h>
#include <string.h>
#include "threadPool.h"
using namespace std ;

class A {
public :
    static int print(int a) {
        cout << "Hello World!" << a << endl ;
        return 6 ;
    }
private :
    int a ;
} ;

int main() {

    map<int, string> ls;
    ls.insert({1, "222"}) ;
    cout << sizeof(ls) << endl ;
    ls.insert({2, "333"}) ;
    cout << sizeof(ls) << endl ;
    ls.insert({3, "444"}) ;
    cout << sizeof(ls) << endl ;
    //char* aa = (char*)(&ls) ; 
    return 0;
}

