#include "serializeParse.h"

Command parseString(string& aa) {
    Command cmm ;
    cmm.ParseFromString(aa) ;
    cout <<"键值:"<<cmm.lob().key() << endl ;
    return cmm ;
}
