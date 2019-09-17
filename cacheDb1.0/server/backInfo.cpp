#include "backInfo.h"

Response backInfo :: notFind()  {
    Response res ;
    res.set_reply("command not found!") ;
    return res ;
}

Response backInfo :: okReply() {
    Response res ;
    res.set_reply("OK") ;
    return res ;
}

Response backInfo :: keyInvalid() {
    Response res ;
    res.set_reply("key invalid!") ;
    return res ;
}

Response backInfo :: processError() {
    Response res ;
    res.set_reply("process error!") ;
    return res ;
}
