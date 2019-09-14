#include "rpc.h"

//处理结果，并返回相应的结果
shared_ptr<Command> rpc :: getParseString(string* buff) {
    //在消息处理处，反序列化
    auto res = parseMethod(buff) ;
    cout << "反序列化！" << res->cmd() << endl ;
    return res ;
}


int rpc :: response(shared_ptr<Response>res, int fd) {
    //回复客户端
    string s ;
    s = res->reply() ;
    //序列化，转化成string
    res->SerializeToString(&s) ;
    char buf[4096] ;
    strcpy(buf, s.c_str()) ;
    //向客户端发送消息
    int ret = send(fd, buf, sizeof(buf), 0) ;
    if(ret < 0) {
        cout << __FILE__ << "      " << __LINE__ << endl ;
        return -1 ;
    }
    cout << "发送回复成功！" << endl ;
    return ret ;
}

///反序列化
shared_ptr<Command> requestMethod(string* s) {
    Command cmd ;
    cmd.ParseFromString(*s) ;
    shared_ptr<Command>comm(new Command(cmd)) ;
    return comm ;
}
