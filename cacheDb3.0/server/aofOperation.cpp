#include "aofOperation.h"

unsigned long aofOperation :: seq = 0 ;
unordered_map<aofKey, map<string, vector<pair<string, string>>>, aofHashFunc, aofEqualFunc>aofOperation:: aofHsetRcd ;
unordered_map<aofKey, map<string, string>, aofHashFunc, aofEqualFunc> aofOperation::aofStrRcd ;

void aofOperation :: stringData(const int num, const shared_ptr<Command>&cmd) {
    aofKey akey ;
    akey.num = num ;
    akey.cmd = cmd->cmd() ; 
    string k = cmd->keys(0).key(0) ;
    string val = cmd->vals(0).val(0) ;
    map<string, string> mm ;
    mm[k] = val ;
    aofStrRcd[akey] = mm ;
    return  ;
}

void aofOperation :: hsetData(const int num, const shared_ptr<Command>&cmd) {
    cout << "正在进行异步持久化!" << endl ;
    //获取键值
    aofKey af ;
    af.num = num ;
    //名令
    af.cmd = cmd->cmd() ;
    //键值
    string key = cmd->keys(0).key(0) ;
    int kLen = cmd->keys_size() ;
    int lv = cmd->vals_size() ;
    vector<pair<string, string>>ls ;
    for(int i=1; i<kLen; i++) {
        int lk = cmd->keys(i).key_size() ;
        int vv = cmd->vals(i-1).val_size() ;
        if(lk != vv) {
            return ;
        }
        for(int j= 0; j<lk; j++) {
            string kk = cmd->keys(i).key(j) ;
            string vv = cmd->vals(i-1).val(j) ;
            ls.push_back({kk, vv}) ;
        }   
    }
    //aofHsetRcd[af] = ls ;
}

