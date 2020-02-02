#include "rpc.h"

rpc :: rpc()  {
    cmd = shared_ptr<Command>(new Command) ;
    parseMethod = requestMethod ;
}


//处理结果，并返回相应的结果
shared_ptr<Command> rpc :: getParseString(const char* buf) {
    //在消息处理处，反序列化
    auto res = parseMethod(buf) ;
    return res ;
}


int rpc :: response(shared_ptr<Response>res, int fd) {
    //回复客户端
    //序列化，转化成string
    char buf[4096] ;
    res->SerializeToArray(buf, 4096) ;
    //向客户端发送消息
    int ret = send(fd, buf, sizeof(buf), 0) ;
    if(ret < 0) {
        cout << __FILE__ << "      " << __LINE__ << "  " << strerror(errno)<< endl ;
        return -1 ;
    }
    return ret ;
}

///反序列化
shared_ptr<Command> requestMethod(const char* buf) {
    Command cmd ;
    cmd.ParseFromArray(buf, 4096) ;
    shared_ptr<Command>comm = make_shared<Command>(cmd) ;
    return comm ;
}
