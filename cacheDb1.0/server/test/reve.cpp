#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int>ls ;
    int i = 0 ;
    for(i=0; i<5; i++) {
        cin >> i ;
        ls.push_back(i) ;
    }
    cout << ls.size() << endl ;
    ls.reserve(20) ;
        
    cout << ls[15] ;
    for(int j= i; j<20; j++) {
        int a ;
        cin >> a ;
        ls.push_back(a) ;
    }
    int len = ls.size() ;
    cout << "数据－－－－－－－》"<<endl  ;
    for(int i=0; i<len; i++) {
        cout << ls[i] << endl ;
    }
    return 0;
}

