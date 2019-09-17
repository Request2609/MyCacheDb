#include <iostream>
#include "stringOperation.h"
#include "serializeParse.h"
#include "msg.pb.h"
using namespace std  ;
using namespace Messages ;

int main() {

    string com ;
    vector<string>ls = split(com, " ");
    int len = ls.size() ;
    Command cmd ;

    string aa = "";
    cmd.set_cmd("get") ;
    cmd.set_key("name") ;
    cmd.set_len(10) ;
    Value* val = cmd.add_vals() ;
    //设置值,值可能重复
    for(int i=2; i<len; i++) {
        string* a = val->add_val() ;
        *a = ls[i] ;
        cout << *a << endl ;
    }   
    string aaa ;
    
    cmd.SerializeToString(&aaa) ;
    //发序列化
    Command cmm ;
    cmm.ParseFromString(aaa) ;
    cout << "cmd:" << cmm.cmd() << endl ;
    cout << "key:" << cmm.key() << endl ;
    len = cmm.vals_size() ;
    for(int i=0; i<len; i++) {
        int a = cmm.vals(i).val_size() ;
        for(int j=0; j<a; j++) {
            cout << "value :" << cmm.vals(i).val(j) << endl;
        }
    }
    cout << "len:" << cmm.len() << endl ;
    cout << "结束" << endl ;
    return 0;
}


