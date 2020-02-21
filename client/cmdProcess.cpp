#include "cmdProcess.h"

void cmdProcess :: setSet(std::vector<std::string>& res, Messages::Command& com) {
    com.set_type(1) ;
    com.set_cmd(res[0]) ;
    Messages::Key* key = com.add_keys() ;
    std::string* k = key->add_key() ;
    *k = res[1] ;
    Messages::Value* val = com.add_vals() ;
    std::string* v = val->add_val() ;
    //设置数据库编号
    *v = res[2] ;
}

void cmdProcess :: setSpop(std::vector<std::string>& res, Messages::Command& cmd) {
    cmd.set_type(1) ;
    cmd.set_cmd(res[0]) ;
    int len = res.size() ;
    Messages::Key* key= cmd.add_keys() ;
    for(int i=1; i<len-1; i++) {
        std::string* k = key->add_key() ;
        *k = res[i] ;
    }
    std::string time = res[len-1] ;
    int t = atoi(time.c_str()) ;
    cmd.set_time(t) ; 
}

void cmdProcess :: getBlpop(std::vector<std::string>& res, Messages::Command& cmd) {
    cmd.set_type(1) ;
    cmd.set_cmd(res[0]) ;
    int len = res.size() ;
    Messages::Key* key= cmd.add_keys() ;
    for(int i=1; i<len-1; i++) {
        std::string* k = key->add_key() ;
        *k = res[i] ;
    }
    std::string time = res[len-1] ;
    int t = atoi(time.c_str()) ;
    cmd.set_time(t) ; 
}

void cmdProcess :: setGet(std::vector<std::string>& res, Messages::Command& cmd) {
    cmd.set_cmd("get") ;
    cmd.set_type(0) ;
    Messages::Key* key = cmd.add_keys() ;
    std::string* k = key->add_key() ;
    *k = res[1] ;
}

void cmdProcess :: setSave(std::vector<std::string>&res,Messages::Command& com) {
    com.set_type(1) ;
    com.set_cmd(res[0]) ;
}


void cmdProcess :: setBgSave(std::vector<std::string>&res,Messages::Command& com) {
    com.set_type(1) ;
    com.set_cmd(res[0]) ;
}
//设置Hget方法
void cmdProcess :: setHget(std::vector<std::string>&res,Messages::Command& com) {
    com.set_type(0) ;
    int len = res.size() ;
    com.set_cmd(res[0]) ;
    Messages::Key* key = com.add_keys() ;
    std::string* ky = key->add_key() ;
    *ky = res[1] ;
    std::string* val = key->add_key() ;  
    *val = res[2] ;
}

void cmdProcess :: setLPopObject(std::vector<std::string>& res, Messages::Command& com) {
    com.set_cmd(res[0]) ;
    com.set_type(1) ;
    Messages::ListObject* lob = com.add_lob() ;
    lob->set_key(res[1]) ;
}

//设置hash
void cmdProcess :: setHset(std::vector<std::string>&res,Messages::Command& com) {
    com.set_type(1) ;
    int len = res.size() ;
    com.set_cmd(res[0]) ;
    std::string cmd = res[0] ;
    Messages::Key* key = com.add_keys() ;
    std::string* k = key->add_key() ;
    *k = res[1] ;
    Messages::Value* val = com.add_vals() ;
    Messages::Key* keyk = com.add_keys() ;
    for(int i=2; i<len; i+=2) {
        std::string* v = val->add_val() ;
        *v = res[i+1] ;
        std::string* k = keyk->add_key() ;
        *k = res[i] ;
    }
}

void cmdProcess :: setZadd(std::vector<std::string>&res, Messages::Command& com) {
    com.set_cmd(res[0]) ;
    Messages::ListObject* lob = com.add_lob() ;
    lob->set_key(res[1]) ;
    Value* val = lob->add_vals() ;
    std::string*s = val->add_val() ;
    *s = res[2] ;
    std::string*s1 = val->add_val() ;
    *s1 = res[3] ;
    return  ;
}

void cmdProcess::setZRange(std::vector<std::string>&res, Messages::Command& com) {
    com.set_type(0) ;
    com.set_cmd(res[0]) ;
    Messages::ListObject*lob = com.add_lob() ;
    lob->set_key(res[1]) ;
    Value* val = lob->add_vals() ;
    std::string * s = val->add_val() ;
    *s = res[2] ;
    std::string* s1 = val->add_val() ;
    *s1 = res[3] ;
    return ;
}

void cmdProcess ::setSadd(std::vector<std::string>& res, Messages::Command& com) {
    com.set_type(0) ;
    Messages::ListObject *lob = com.add_lob() ;
    com.set_cmd(res[0]) ;
    lob->set_key(res[1]) ;
    int len = res.size() ;
    Value* val = lob->add_vals() ;
    for(int i=2; i<len; i++) {
        std::string*v = val->add_val() ;
        *v = res[i] ;
    }
}

void cmdProcess :: getListObject(std::vector<std::string>& res, Messages::Command& com) {
    com.set_type(0) ;
    Messages::ListObject *lob = com.add_lob() ;
    com.set_cmd(res[0]) ;
    lob->set_key(res[1]) ;
    int len = res.size() ;
    Value* val = lob->add_vals() ;
    for(int i=2; i<len; i++) {
        std::string*v = val->add_val() ;
        *v = res[i] ;
    }
}
