#include "backInfo.h"

shared_ptr<Response> backInfo :: notFind()  {
    shared_ptr<Response> res =shared_ptr<Response>(new Response);
    res->set_reply("command not found!") ;
    return res ;
}

shared_ptr<Response> backInfo :: okReply() {
    shared_ptr<Response> res =shared_ptr<Response>(new Response);
    res->set_reply("OK") ;
    return res ;
}

shared_ptr<Response> backInfo :: keyInvalid() {
    shared_ptr<Response> res =shared_ptr<Response>(new Response);
    res->set_reply("key invalid!") ;
    return res ;
}

shared_ptr<Response> backInfo :: processError() {
    shared_ptr<Response> res =shared_ptr<Response>(new Response);
    res->set_reply("process error!") ;
    return res ;
}
