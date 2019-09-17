#pragma once
#include <iostream>
#include <vector>
#include "aeEvent.h"
#include "msg.pb.h"

using namespace Messages ;
using namespace std ;

//序列化和反序列化调用的方法,并将消息发送给客户端
void method(shared_ptr<Command>cmd, shared_ptr<aeEvent>event, string& res) {
    
}
