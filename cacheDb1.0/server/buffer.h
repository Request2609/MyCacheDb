#pragma once
#include <string>
#include <vector>
using namespace std ;

class buffer
{
public:
    buffer() {
        readIndex = 0 ;
        writeIndex = 0 ;
    }
    ~buffer() {}
public :
    void append(char cc) { 
        buf.push_back(cc) ;
        readIndex ++ ; 
    }
    string* getBuf() {
        return &buf ;
    }
    char get() { 
        return buf[writeIndex++] ;
    }
    
    void clear() { 
        readIndex = 0;
        writeIndex = 0 ;
        buf.clear() ;
    }

private:
    string buf ;
    long readIndex ;
    long writeIndex ;
};

