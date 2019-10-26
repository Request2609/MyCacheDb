#include <iostream>
#include <future>
#include <thread>

using namespace std ;

int print(int i) {
    cout << "传进来的值:" << i << endl ;
    return i+2 ;
}   
int main() {
    ///包装函数
    packaged_task<int(int)> func(&print) ;
    auto res = func.get_future() ;
    thread t(move(func), 10) ;
    cout << res.get() << endl ;
    t.join() ;
    return 0;
}

