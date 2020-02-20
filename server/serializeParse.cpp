#include "serializeParse.h"

Messages::Command parseString(std::string& aa) {
    Command cmm ;
    cmm.ParseFromString(aa) ;
    return cmm ;
}
