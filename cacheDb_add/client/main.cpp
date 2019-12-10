#include <iostream>
#include "clientLoop.h"
#include "rpc.h"
#include "request.h"
using namespace std ;

Response parseMethod(string* s) {
    Response res ;
    res.ParseFromString(*s) ;
    return res ;
}

int main(int argc, char** argv) {
    
    //初始化令表
//    cmds::build() ;
    rpc rc ;
    if(argc != 3) {
        cout << __FILE__ << "        " << __LINE__ << endl ;
        return 1 ;
    }

    rc.setCallMethod(request :: sendReq) ;
    clientLoop clp(&rc) ;
    //设置反序列化回调
    rc.setCallMethod(parseMethod) ;
    
    clp.start(argv[1], argv[2]) ;
    return 0;
}

