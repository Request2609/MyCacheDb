#pragma once
#include <iostream>
#include "msg.pb.h"
#include "cmdProcess.h"
using namespace std ;
using namespace Messages ;

class backInfo {
public:
    backInfo() {}
    ~backInfo() {}
public :
    static Response notFind() ;
    static Response okReply() ;
    static Response keyInvalid() ;
    static Response processError() ;

} ;
