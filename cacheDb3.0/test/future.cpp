#include <iostream>
#include <thread>
#include <future>
using namespace std ;
int print() {
    cout << "hello world!" << endl ;
    return 1 ;
}

int main()
{
    thread tt(print);

    return 0;
}

