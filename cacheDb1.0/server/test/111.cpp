#include <iostream>
#include <fstream>
using namespace std ;

int main() {
    ofstream out("test.bin", ios::out|ios::binary) ;
    if(out.fail()) {
        cout << __LINE__ << "   " << __FILE__ << endl ;
        return -1 ;
    }
    const char* f = "hello world!" ;
    out << f ;
    out.close() ;
    getchar() ;
    ifstream in("test.bin", ios::in|ios::binary) ;
    char a[100] ;
    char b[100] ;
    in >> a >>b;
    cout << a <<b<< endl ;
    in.close() ; 
    return 0;
}

