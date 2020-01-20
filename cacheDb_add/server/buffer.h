#pragma once
#include <string>
#include <string.h>
#include <vector>
using namespace std ;

class buffer {
public:
    buffer() {
        readIndex = 0 ;
        writeIndex = 0 ;
    }
    ~buffer() {}
public :
    void init() {
        bzero(buf,  sizeof(buf)) ;
    }
    void append(char* cc) { 
        memcpy(buf, cc, 4096-1) ;   
    }
    char* getBuf() {
        return buf ;
    }
    char get() { 
        return buf[writeIndex++] ;
    }
    
    void clear() { 
        readIndex = 0;
        writeIndex = 0 ;
    }

private:
    char buf[4096] ;
  //  string buf ;
    long readIndex ;
    long writeIndex ;
};

