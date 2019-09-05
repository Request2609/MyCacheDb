#include "serializeParse.h"

Command parseString(string& aa) {
    Command cmm ;
    cmm.ParseFromString(aa) ;
    
    return cmm ;
}
