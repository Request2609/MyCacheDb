#include <iostream>
#include <string>
using namespace std ;

int main() {
    string aa = "hello world!" ;
    cout << aa.find("hello") << endl ;
    if(aa.find("whll") == string::npos) {
        cout << "没找到!" << endl ;

    }

    return 0;
}

