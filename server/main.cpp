#include "aeEventloop.h"
#include "aeEvent.h"
#include "cmdProcess.h"

int readOnMessage(shared_ptr<aeEvent>tmp) { 
    //处理事件    
    static cmdProcess cmdPro ;
    //设置rpc
    static shared_ptr<rpc>rc = make_shared<rpc>() ;
    cmdPro.setRpc(rc) ;
    //处理消息
    cout <<"消息："  << tmp->getBuf()->getBuf() << endl ;
    int ret = cmdPro.processMsg(tmp) ;    
    return ret ;
}

int main(int argc, char** argv) {

    if(argc != 3) {
        cout << "usage:<./a.out><ip><port>" << endl ;
        return 1 ;
    }
    aeEventloop aeLoop ;
    //设置读回调函数
    aeLoop.setReadCallBack(readOnMessage) ;
    aeLoop.addServerEvent(argv[1], argv[2]) ;
    aeLoop.start() ;
    return 0;
}

