#pragma once
#include <iostream>
#include <vector>
#include <string.h>
#include "msg.pb.h"

using namespace std ;
using namespace Messages ;


class cmdProcess
{
public:
    cmdProcess() {}
    ~cmdProcess() {}
    static void  setGet(vector<string>& res, Command& cmd) ;
    static void setSet(vector<string>&res, Command&com) ;
    static void setBgSave(vector<string>&res,Command& com) ;
    static void setHset(vector<string>&res,Command& com) ;
    static void setHget(vector<string>&res,Command& com) ;
private:

};

