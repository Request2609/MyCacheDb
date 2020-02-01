#pragma once
#include <iostream>
#include <sys/time.h>
#include <stdlib.h>
#include <unordered_map>
#include <mutex>
#include <memory>
#include <queue>
using namespace std ;
class syncQueue {
public:
    ~syncQueue() {}
    static shared_ptr<syncQueue>getQueue() ;
    void addToResponse(long num, const string s) ;
    long addToQueue() ;
    void getResponse() ;
    int reqEmpty() ;
    int resEmpty() { return response.size() ; }
    int getResponse(long) ;
private:
    void addRequest() ;
    syncQueue() {}
    static shared_ptr<syncQueue>que ;
    queue<long> req ;
    unordered_map<long, string> response ;
    mutex mute;
};

