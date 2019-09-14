#include <iostream>
#include <fstream>
using namespace std ;

int main() {
    
    long size ;
    ifstream in("hello", ios::binary|ios::ate) ;
    size = in.tellg() ;
    in.seekg(0, ios::beg) ;
    char* buffer = new char[size] ;
    in.read(buffer, size-1) ;
    in.close() ;
    cout << buffer << endl ;
    delete[] buffer ;
    return 0;
}

