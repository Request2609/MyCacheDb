#include <iostream>
#include <vector>
using namespace std ;

class person {
public :
    virtual void print() ;
    void say() ;
} ;

class son :public person{
public :
    void print() {
        cout << "hello world!" << endl ;     
    }
    void say() {
        cout << "wwasasjjd" << endl ;
    }
} ;
int main() {
    vector<son>a ;
    person p;
    a.push_back(p) ;
    p.print() ;
    p.say() ;
    return 0;
}

