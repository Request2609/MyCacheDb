#pragma once
#include <iostream>
#include "cmdProcess.h"
#include "msg.pb.h"


class backInfo {
public:
    backInfo() {}
    ~backInfo() {}
public :
    static std::shared_ptr<Messages::Response> notFind() ;
    static std::shared_ptr<Messages::Response> okReply() ;
    static std::shared_ptr<Messages::Response> keyInvalid() ;
    static std::shared_ptr<Messages::Response> processError() ;
} ;
