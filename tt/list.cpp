#include <iostream>
#include <list>
using namespace std ;

struct A {
    int a ;
};
int main() {
    list<A>ls ;
    A aa ;
    aa.a=1 ;
    ls.push_back(aa) ;
    aa.a=2 ;
    ls.push_back(aa) ;
    aa.a=5 ;
    ls.push_back(aa) ;
    list<A>::iterator iter = ls.begin();
    list<A>::reverse_iterator iters = ls.rbegin() ;
    ls.insert(iters, aa) ;
    for(auto s=ls.begin(); s!=ls.end(); s++) {
        if((*s).a == 5) {
            A b ;
            b.a = 4 ;
            ls.insert(iter, b) ;
    
            break ;
        }
        iter = s ;
    }
    for(auto s:ls) {
        cout << s.a << "   " ;
    }
    cout << endl ;
    return 0;
}

