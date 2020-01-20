#include "checkOperation.h"

shared_ptr<checkOperation> checkOperation::chk = nullptr ;

shared_ptr<checkOperation> checkOperation:: getCheckObject() {
    if(chk == nullptr)
    chk =shared_ptr<checkOperation>(new chec) ;
    else return chk ;
}


