#include <iostream>
#include <string>
#include <unordered_map>
using namespace std ;

class AA ;

class AA {
public :
    int a ;
    string b ;
    double c ;
} ;

class hashFunc {
public :
    std :: size_t operator()(const AA& aa) {
        return (((hash<int>()(aa.a)<<1)^(hash<string>()(aa.b)<<1))>>1)^(hash<double>()(aa.c) >> 1) ;
    }
} ;

class equalFunnc {
public :
    bool operator() (const AA& aa1, const AA& aa2) {
        return aa1.a == aa2.a && aa1.b == aa2.b && aa1.c == aa2.c ;
    }
} ;
int main()
{
    unordered_map<AA, string> mm ;
    AA aa ;
    aa.a = 1 ;
    aa.c = 1.9 ;
    aa.b = "ccc" ;
    mm.insert({aa, "oooo"}) ;
    AA bb ;
    bb.a = 1 ;
    bb.b = "9999" ;
    bb.c = 1.9 ;    
    mm.insert({bb, "fewfe"}) ;
    for(auto j : mm) {
        cout << j.first << "     " << j.second << endl ;
    }
    return 0;
}

