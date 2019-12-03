#pragma once
#include <iostream>
#include <sys/time.h>
using namespace std ;

class timer { 
public:
    timer() {}
    ~timer() {}
    static long getCurTime() ;
};

