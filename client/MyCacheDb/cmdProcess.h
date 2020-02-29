#pragma once
#include <iostream>
#include <vector>
#include <string.h>
#include "msg.pb.h"



class cmdProcess
{
public:
    cmdProcess() {}
    ~cmdProcess() {}
    static void  setGet(std::vector<std::string>& res, Messages::Command& cmd) ;
    static void setSet(std::vector<std::string>&res, Messages::Command&com) ;
    static void setSave(std::vector<std::string>&res,Messages::Command& com) ;
    static void setHset(std::vector<std::string>&res,Messages::Command& com) ;
    static void setBgSave(std::vector<std::string>&res,Messages::Command& com) ;
    static void setHget(std::vector<std::string>&res,Messages::Command& com) ;
    static void getBlpop(std::vector<std::string>& res, Messages::Command& com) ;
    static void getListObject(std::vector<std::string>& res, Messages::Command& com) ;
    static void setLPopObject(std::vector<std::string>&res, Messages::Command& com) ;
    static void setZadd(std::vector<std::string>&res, Messages::Command& com) ;
    static void setZRange(std::vector<std::string>&res, Messages::Command& com) ;
    static void setSadd(std::vector<std::string>&res, Messages::Command& com) ;
    static void setSpop(std::vector<std::string>&res, Messages::Command& com) ;
private:
    
};

