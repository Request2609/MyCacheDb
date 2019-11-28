#pragma once
#include <mutex>
#include <iostream>
using namespace std ;

class checkExpire {
public:
    checkExpire() {}
    ~checkExpire() {}
    
private:
    static mutex mute ;
    
};

