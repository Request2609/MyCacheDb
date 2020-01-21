#include "checkOperation.h"

shared_ptr<checkOperation> checkOperation::chk = nullptr ;

shared_ptr<checkOperation> checkOperation:: getCheckObject() {
    if(chk == nullptr)
    chk =shared_ptr<checkOperation>(new checkOperation()) ;
    else return chk ;
}


void checkOperation::getSaveRule() {
    ifstream  in ;
    in.open("../config/save", ios::in) ;
    if(in.fail()) {
        cout << __LINE__ << "    " << __FILE__ << endl ;
        return ;
    }
    string s ;
    while(!in.eof()) {
        int sec=0, time=0 ;
        in>>s>> sec>> time ;
        modifyMap.insert({sec, time}) ;
    }
    in.close() ;
}


