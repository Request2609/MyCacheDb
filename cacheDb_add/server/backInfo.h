#pragma once
#include <iostream>
#include "cmdProcess.h"
#include "msg.pb.h"
using namespace std ;
using namespace Messages ;


class backInfo {
public:
    backInfo() {}
    ~backInfo() {}
public :
    static shared_ptr<Response> notFind() ;
    static shared_ptr<Response> okReply() ;
    static shared_ptr<Response> keyInvalid() ;
    static shared_ptr<Response> processError() ;
} ;
