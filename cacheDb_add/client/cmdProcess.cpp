#include "cmdProcess.h"

void cmdProcess :: setSet(vector<string>& res, Command& com) {
    com.set_type(1) ;
    com.set_cmd(res[0]) ;
    Key* key = com.add_keys() ;
    string* k = key->add_key() ;
    *k = res[1] ;
    Value* val = com.add_vals() ;
    string* v = val->add_val() ;
    //设置数据库编号
  //  com.set_num(0) ;
    *v = res[2] ;
}

void cmdProcess :: setSpop(vector<string>& res, Command& cmd) {
    cmd.set_type(1) ;
    cmd.set_cmd(res[0]) ;
    int len = res.size() ;
    Key* key= cmd.add_keys() ;
    for(int i=1; i<len-1; i++) {
        string* k = key->add_key() ;
        *k = res[i] ;
    }
    string time = res[len-1] ;
    int t = atoi(time.c_str()) ;
    cmd.set_time(t) ; 
}

void cmdProcess :: getBlpop(vector<string>& res, Command& cmd) {
    cmd.set_type(1) ;
    cmd.set_cmd(res[0]) ;
    int len = res.size() ;
    Key* key= cmd.add_keys() ;
    for(int i=1; i<len-1; i++) {
        string* k = key->add_key() ;
        *k = res[i] ;
    }
    string time = res[len-1] ;
    int t = atoi(time.c_str()) ;
    cmd.set_time(t) ; 
}

void cmdProcess :: setGet(vector<string>& res, Command& cmd) {
    cmd.set_cmd("get") ;
    cmd.set_type(0) ;
    Key* key = cmd.add_keys() ;
    string* k = key->add_key() ;
    *k = res[1] ;
}

void cmdProcess :: setSave(vector<string>&res,Command& com) {
    com.set_type(1) ;
    com.set_cmd(res[0]) ;
}


void cmdProcess :: setBgSave(vector<string>&res,Command& com) {
    com.set_type(1) ;
    com.set_cmd(res[0]) ;
}
//设置Hget方法
void cmdProcess :: setHget(vector<string>&res,Command& com) {
    com.set_type(0) ;
    int len = res.size() ;
    com.set_cmd(res[0]) ;
    Key* key = com.add_keys() ;
    string* ky = key->add_key() ;
    *ky = res[1] ;
    string* val = key->add_key() ;  
    *val = res[2] ;
}

void cmdProcess :: setLPopObject(vector<string>& res, Command& com) {
    com.set_cmd(res[0]) ;
    com.set_type(1) ;
    ListObject* lob = com.add_lob() ;
    lob->set_key(res[1]) ;
}

//设置hash
void cmdProcess :: setHset(vector<string>&res,Command& com) {
    com.set_type(1) ;
    int len = res.size() ;
    com.set_cmd(res[0]) ;
    string cmd = res[0] ;
    Key* key = com.add_keys() ;
    string* k = key->add_key() ;
    *k = res[1] ;
    Value* val = com.add_vals() ;
    Key* keyk = com.add_keys() ;
    for(int i=2; i<len; i+=2) {
        string* v = val->add_val() ;
        *v = res[i+1] ;
        string* k = keyk->add_key() ;
        *k = res[i] ;
    }
}

void cmdProcess :: setZadd(vector<string>&res, Command& com) {
    com.set_cmd(res[0]) ;
    ListObject* lob = com.add_lob() ;
    lob->set_key(res[1]) ;
    Value* val = lob->add_vals() ;
    string*s = val->add_val() ;
    *s = res[2] ;
    string*s1 = val->add_val() ;
    *s1 = res[3] ;
    return  ;
}

void cmdProcess::setZRange(vector<string>&res, Command& com) {
    com.set_type(0) ;
    com.set_cmd(res[0]) ;
    ListObject*lob = com.add_lob() ;
    lob->set_key(res[1]) ;
    Value* val = lob->add_vals() ;
    string * s = val->add_val() ;
    *s = res[2] ;
    string* s1 = val->add_val() ;
    *s1 = res[3] ;
    return ;
}

void cmdProcess ::setSadd(vector<string>& res, Command& com) {
    com.set_type(0) ;
    ListObject *lob = com.add_lob() ;
    com.set_cmd(res[0]) ;
    lob->set_key(res[1]) ;
    int len = res.size() ;
    Value* val = lob->add_vals() ;
    for(int i=2; i<len; i++) {
        string*v = val->add_val() ;
        *v = res[i] ;
    }
}

void cmdProcess :: getListObject(vector<string>& res, Command& com) {
    com.set_type(0) ;
    ListObject *lob = com.add_lob() ;
    com.set_cmd(res[0]) ;
    lob->set_key(res[1]) ;
    int len = res.size() ;
    Value* val = lob->add_vals() ;
    for(int i=2; i<len; i++) {
        string*v = val->add_val() ;
        *v = res[i] ;
    }
}
