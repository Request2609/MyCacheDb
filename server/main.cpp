#include <signal.h>
#include "aeEventloop.h"
#include "aeEvent.h"
#include "cmdProcess.h"

//保证只创建一次
static cmdProcess cmdPro ;
static std::shared_ptr<rpc>rc = nullptr ;

static int readOnMessage(std::shared_ptr<aeEvent>tmp) { 
    if(rc == nullptr) {
        //从文件中读取数据库中的数据
        cmdPro.initRedis() ; 
        //初始化命令集合
        cmdPro.initCmdCb() ;
    }
    //设置序列化和反序列化句柄
    rc = make_shared<rpc>() ;
    cmdPro.setRpc(rc) ;
    //处理消息
    int ret = cmdPro.processMsg(tmp) ;
    return ret ;
}
int main(int argc, char** argv) {
    string ip ;
    string port ; 
    signal(SIGPIPE, SIG_IGN) ;
    if(argc < 3) {
        getIpPort(ip, port) ;
    } 
    else {
        ip = argv[1] ;
        port = argv[2] ;
    }
    aeEventloop aeLoop ;
    aeLoop.init() ;
    //设置读回调函数
    aeLoop.setReadCallBack(readOnMessage) ;
    aeLoop.addServerEvent(ip, port) ;
    aeLoop.start() ;
    return 0;
}

