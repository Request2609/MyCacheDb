#pragma once
#include <iostream>
#include "cmdProcess.h"
#include "msg.pb.h"
using namespace Messages ;


class backInfo {
public:
    backInfo() {}
    ~backInfo() {}
public :
    static std::shared_ptr<Response> notFind() ;
    static std::shared_ptr<Response> okReply() ;
    static std::shared_ptr<Response> keyInvalid() ;
    static std::shared_ptr<Response> processError() ;
} ;
