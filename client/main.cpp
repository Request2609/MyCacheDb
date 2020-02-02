#include <iostream>
#include "clientLoop.h"
using namespace std ;
int main(int argc, char** argv) {
    clientLoop clp ;
    //传入IP 和地址
    clp.init("127.0.0.1", "8888") ;
    clp.start() ;
    return 0;
}

