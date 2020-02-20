#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "msg.pb.h"
using namespace Messages ;


int  serializeToString(Command cmd, std::vector<std::string>ls, std::string* s) ;
Command parseString(std::string& aa) ;
