#include "backInfo.h"

std::shared_ptr<Messages::Response> backInfo :: notFind()  {
    std::shared_ptr<Messages::Response> res =std::shared_ptr<Response>(new Response);
    res->set_reply("command not found!") ;
    return res ;
}

std::shared_ptr<Messages::Response> backInfo :: okReply() {
    std::shared_ptr<Messages::Response> res =std::shared_ptr<Response>(new Response);
    res->set_reply("OK") ;
    return res ;
}

std::shared_ptr<Messages::Response> backInfo :: keyInvalid() {
    std::shared_ptr<Messages::Response> res =std::shared_ptr<Response>(new Response);
    res->set_reply("key invalid!") ;
    return res ;
}

std::shared_ptr<Messages::Response> backInfo :: processError() {
    std::shared_ptr<Messages::Response> res =std::shared_ptr<Response>(new Response);
    res->set_reply("process error!") ;
    return res ;
}
