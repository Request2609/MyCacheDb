#include <iostream>
#include <google/dense_hash_map>
using namespace google ;
using namespace std ;

int main() {
    
    dense_hash_map<int, int> ls;
    ls[1] = 2 ;
    ls[3] = 5 ;
    ls[6] = 9 ;
    for(auto iter = ls.begin(); iter !=ls.end(); iter++) {
        cout << iter->second << endl ;
   //     if(iter->first == 1) {
     //       ls.insert({7,8}) ;
       // }
    }
    return 0;
}

