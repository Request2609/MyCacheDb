#include <iostream>
#include <vector>
using namespace std ;

class A {
public :
    virtual vector<string>print() = 0;
    virtual void prints() = 0 ;
} ;

class B : public A {
public :
    vector<string>print() {
        vector<string>ls ;
        ls.push_back("hhh") ;
        return ls ;
    }
    void prints() {
    }
} ;

int main() {
    
    return 0;
}

