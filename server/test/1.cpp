#include <iostream>
#include <memory>
#include <vector>
using namespace std ;

class B {
public :
    B() {} 
    ~B(){}
public :
    int getB() {
        return b ;
    }
    void setB(int i) {
        b = i ;
    }
private :
    int b ;
}; 

class A {
public :
    A(){} 
    ~A() {}
public :
    int input(shared_ptr<B>b) { 
        bb.push_back(b);
        print() ;
        return 1 ;
    }

    void print() {
        cout << "=+++++++++++++++++++++++++++++" << endl ;
        int len = bb.size() ;
        for(int i=0; i<len; i++) {
            cout<<bb[i]->getB()<< endl ;
        }
    }
private :
    vector<shared_ptr<B>>bb ;
    int a ;
};

void example(shared_ptr<B>b) {
    static A aaa ;
    aaa.input(b) ;
}

int main() {
    shared_ptr<B> b = make_shared<B>() ;
    b->setB(10) ;
    example(b) ;
    shared_ptr<B> bb = make_shared<B>();
    bb->setB(9) ;
    example(bb) ;
    return 0;
}

