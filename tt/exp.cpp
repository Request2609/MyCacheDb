#include <iostream>
#include <string>
using namespace std ;
int main()
{
    string ss="hello " ;
    if(string::npos==ss.find("pp")) {
        cout << "jjjj " << endl ;
    }
    return 0;
}

