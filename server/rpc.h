#pragma once
#include <iostream>
#include <memory>                                                                                                                                                                                                                                                                                                                                                                            
#include <functional>
#include "aeEvent.h"
#include "msg.pb.h"
class rpc ;
std::shared_ptr<Messages::Command> requestMethod(const char* s) ;

class rpc {
    typedef std::function<void(std::shared_ptr<Messages::Response>res, int fd)> request ;
    typedef std::function<std::shared_ptr<Messages::Command>(const char*)> parse ;
public:
    rpc() ;
    ~rpc() {
    }
public :
    static std::string getParseString() ;
    //设置相应的callMethod方法
    std::shared_ptr<Messages::Command>getParseString(const char* buff) ;
    int response(std::shared_ptr<Messages::Response>res, int fd) ;
    //设置相应的callMethod方法
    //反序列化函数
    void setCallMethod(parse par) {
        parseMethod = move(par) ;
    }
    ///反序列化
private :
    std::shared_ptr<Messages::Command>cmd ;
    //   request requestMethod ;
    //反序列化函数
    parse parseMethod ;
    int a ;
}
;
