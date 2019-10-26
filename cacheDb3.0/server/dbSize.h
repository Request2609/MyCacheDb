#pragma once
#include <iostream>
#include "msg.pb.cpp"

using namespace std ;

class dbSize {
public:
    dbSize(){}
    ~dbSize() {}
public :
    static void addSize(unsigned long& s) ;
    static void releaseSize(unsigned long& s) ;
    static unsigned long long  getSize() ;
    static int getMB() ;
    static int getGB() ;
private:
    //记录数据库占用内存的总大小
    static unsigned long long size ;
};


