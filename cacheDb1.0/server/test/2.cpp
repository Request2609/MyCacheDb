#include <iostream>
#include <vector>
using namespace std ;

int main() {
    vector<int>ls ;
    ls.reserve(10) ;
    for(int i=0; i<10; i++) {
        ls.push_back(i) ;
    }
    ls.reserve(20) ;
    for(int i=10; i<20; i++) {
        ls.push_back(i) ;
    }
    for(int i=0; i<20; i++) {
        cout << ls[i] << endl ;
    }
    return 0;
}

