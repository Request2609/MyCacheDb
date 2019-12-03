#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "msg.pb.h"
using namespace std ;
using namespace Messages ;


int  serializeToString(Command cmd, vector<string>ls, string* s) ;
Command parseString(string& aa) ;
