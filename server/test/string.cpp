#include <iostream>
using namespace std ;
int main()
{
    string  s = " hellow" ;
    long l = s.find("word") ;
    if(l == (long)string :: npos) {
        cout << "NOT FIND" << endl ;
    }
    else 
    std::cout << "Hello world" << std::endl;
    return 0;
}

