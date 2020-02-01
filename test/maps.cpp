#include <iostream>
#include <map>
using namespace std ;
int main()
{
    map<int, string> ls ;
    ls[1] = "hello" ;
    ls[2] = "world" ;
    ls[3] = "he" ;
    cout << ls.size() << endl ;
    std::cout << "Hello world" << std::endl;
    return 0;
}

