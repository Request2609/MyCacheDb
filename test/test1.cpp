#include <iostream>
#include <vector>
#include <list>

using namespace std ;

int main()
{
    list<int>s ;
    s.push_back(1) ;
    s.push_back(2) ;
    s.push_back(3) ;
    s.push_back(1) ;
    s.push_back(1) ;
    cout << s.size() << endl ;
    s.remove(1) ;
    cout << s.size() << endl ;
    return 0;
}

