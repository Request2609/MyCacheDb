#include <iostream>
#include <libclient/clientLoop.h>
using namespace std ;
int main(int argc, char** argv) {
    clientLoop clp ;
    //传入IP 和地址
    clp.init("127.0.0.1", "8888") ;
   //在配置文件中找
    //clp.init() ;
//////////////////////////////程序中使用
    clp.sendRequest("set", "name", "wc", END) ;
    clp.sendRequest("get", "name", END) ;
    clp.sendRequest("hset", "student", "name","ck", "age", "18", END) ;
    clp.sendRequest("hget", "student", "name", END) ;
    clp.sendRequest("hget", "student", "age", END) ;
    //........   

    //////////////////////////获取结果
    cout << "获取的返回结果：" << endl ;
    clp.getResult() ;
    clp.getResult() ;
    clp.getResult() ;
    clp.getResult() ;
    clp.getResult() ;

///////////////////////////////////////////////终端使用
//    clp.start() ;
    return 0;
}

