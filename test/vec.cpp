#include <iostream>
#include <vector>
using namespace std ;

int main() {
    vector<int>ls ;
    while(1) {
        int a ;
        cin>> a ;
        if(a == -1)break ;
        ls.push_back(a) ;
    }
    while(!ls.empty())  {
        int a = ls.pop_back() ;

    }
    return 0;
}

