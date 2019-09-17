#include <iostream>
#include <map>
using namespace std ;

int main() {
    pair<int, int> pp = {1, 1} ;
    map<pair<int,string>, pair<int, int>> ll {
        {{1,"hello"}, {1, 1}} ,
            {{1, "world"}, {1, 1}} 
    } ;
    map<pair<int, string>, pair<int, int>> ::iterator iter ;
    for(iter = ll.begin(); iter != ll.end() ; iter++) {
    }
    return 0;
}

