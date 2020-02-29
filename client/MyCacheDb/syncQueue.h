#pragma once
#include <iostream>
#include <sys/time.h>
#include <stdlib.h>
#include <unordered_map>
#include <mutex>
#include <memory>
#include <queue>

class syncQueue {
public:
    ~syncQueue() {}
    static std::shared_ptr<syncQueue>getQueue() ;
    void addToResponse(long num, const std::string s) ;
    long addToQueue() ;
    void getResponse() ;
    int reqEmpty() ;
    int resEmpty() { return response.size() ; }
    int getResponse(long) ;
private:
    void addRequest() ;
    syncQueue() {}
    static std::shared_ptr<syncQueue>que ;
    std::queue<long> req ;
    std::unordered_map<long, std::string> response ;
    std::mutex mute;
};

