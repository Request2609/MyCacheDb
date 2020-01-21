#pragma once
#include <iostream>
#include <vector>
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

class logErr {
public:
    ~logErr() {}
    static void init() ;
    static void record(std::string s) ;
private:
    logErr() {}
};

