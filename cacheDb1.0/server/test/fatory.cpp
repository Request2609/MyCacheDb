#include <iostream>
using namespace std ;

class AA {
public :
    AA() {}
    virtual ~AA(){} 
    virtual void print(){} ;
} ;

class A:public AA{
public :
    A() {}
   ~ A(){} 
   void print() {
       cout << "I am A" << endl ;
   }
} ;

class A1:public AA{
public :
    A1() {}
    ~A1() {}
public :
    void print() {
        cout << "I am A1" << endl;
    }
} ;

class factory {
public :
    factory() {}
    ~factory() {}
    static AA* getA(int num) {
        if(num == 0) {
            A* a = new A() ;
            return a ;
        }
        else {
            A1* a1 = new A1() ;
            return a1 ;
        }
    }
};
int main()
{

    AA* a = factory::getA(0) ;
    a->print();
    AA* a1 = factory::getA(1) ;
    a1->print();
    delete a ;
    delete a1 ;
    return 0;
}

