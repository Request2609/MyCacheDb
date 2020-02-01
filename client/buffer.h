#pragma once
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
        ls.push_back(cc) ;
        readIndex ++ ; 
    }
    char get() { 
        return ls[writeIndex++] ;
    }
    
    void clear() { 
        readIndex = 0;
        writeIndex = 0 ;
        ls.clear() ;
    }

private:
    vector<char>ls ;
    long readIndex ;
    long writeIndex ;
};

