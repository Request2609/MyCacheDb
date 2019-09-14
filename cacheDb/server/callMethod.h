#pragma once
#include <memory>
#include <iostream>
#include "msg.pb.h"
using namespace std ;
using namespace Messages ;

//反序列化的接口实现
Command getParseString(string* aa) {
    Command cmm ;
    cmm.ParseFromString(*aa) ;
    return cmm ;
}

