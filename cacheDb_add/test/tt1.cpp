#include <iostream>
using namespace std ;

class A{
public :
    int operator() (int a, int b){
        return a-b ;
    }
} ;



int main()
{
    int a=1, b= 2 ;
    A a1 ;
    int ret = a1(a, b) ;
    cout << ret << endl ;
    return 0;
}

