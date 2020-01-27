#include <iostream>
#include <vector>
#include <fstream>
using namespace std ;

int main() {
    ofstream out("test", ios::out|ios::binary|ios::app) ;
    if(out.fail()){
        cout << __FILE__ << "     " << __LINE__ << endl ;
        return 1; 
    }
    out << "hellojhjajidhweufhewouf" << endl ;
    out.close() ;
    return 0;
}

