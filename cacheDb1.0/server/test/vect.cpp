#include <iostream>
#include <vector>
using namespace std ;

int main()
{
    vector<int>ls ;
    ls.reserve(10) ;
    ls[4] = 1 ;
    for(int i=0; i<100; i++) {
        ls.push_back(i) ;
    }
    return 0;
}

