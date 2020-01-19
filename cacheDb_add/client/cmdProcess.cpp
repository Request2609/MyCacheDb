#include "cmdProcess.h"

void cmdProcess :: setSet(vector<string>& res, Command& com) {
    com.set_cmd(res[0]) ;
    Key* key = com.add_keys() ;
    string* k = key->add_key() ;
    *k = res[1] ;
    Value* val = com.add_vals() ;
    string* v = val->add_val() ;
    //设置数据库编号
    com.set_num(0) ;
    *v = res[2] ;
}

void cmdProcess :: getBlpop(vector<string>& res, Command& cmd) {
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
    Key* key = cmd.add_keys() ;
    string* k = key->add_key() ;
    *k = res[1] ;
}

void cmdProcess :: setSave(vector<string>&res,Command& com) {
    com.set_cmd(res[0]) ;
}


void cmdProcess :: setBgSave(vector<string>&res,Command& com) {
    com.set_cmd(res[0]) ;
}
//设置Hget方法
void cmdProcess :: setHget(vector<string>&res,Command& com) {

    int len = res.size() ;
    com.set_cmd(res[0]) ;
    Key* key = com.add_keys() ;
    string* ky = key->add_key() ;
    *ky = res[1] ;
    string* val = key->add_key() ;  
    *val = res[2] ;
}

//设置hash
void cmdProcess :: setHset(vector<string>&res,Command& com) {
    int len = res.size() ;
    com.set_cmd(res[0]) ;
    string cmd = res[0] ;
    //string keys = res[1] ;
    com.set_cmd(cmd) ;
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

void cmdProcess :: getListObject(vector<string>& res, Command& com) {
        
    ListObject *lob = com.add_lob() ;
    com.set_cmd(res[0]) ;
    lob->set_key(res[1]) ;
    cout << "设置键："<< lob->key() << endl ; 
    int len = res.size() ;
    Value* val = lob->add_vals() ;
    cout << "设置值:" << endl ;
    for(int i=2; i<len; i++) {
        cout << res[i] << endl ;
        string*v = val->add_val() ;
        *v = res[i] ;
    }
}
