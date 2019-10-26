#include <iostream>
#include <vector>
#include <map>
using namespace std ;

int main()
{
    vector<int>vec ;
    map<string, vector<int>>ls ;
    vec.push_back(1) ;
    ls["hh"] = vec ;
    cout << sizeof(vec) << endl ;
    return 0;
}

