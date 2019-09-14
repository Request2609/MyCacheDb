#include <iostream>
#include "stringOperation.h"
#include "serializeString.h"
#include "msg.pb.h"
//bool SerializeToString(string* output) ;   
//bool ParseFromString(const string& data);  
using namespace std  ;
using namespace Messages ;

int main() {

    string com ;
    getline(cin, com) ;
    vector<string>ls = split(com, " ");
    int len = ls.size() ;
    Command cmd ;

    string aa = "";
    serializeToString(cmd, ls, &aa) ;
   /* cmd.set_cmd(ls[0]);
    cmd.set_key(ls[1]) ;
    Value* val = cmd.add_vals() ;
    for(int i=2; i<len; i++) {
        string*a = val->add_val() ;
        *a = ls[i] ;
    }

    string aa ;*/
   // cmd.SerializeToString(&aa) ;
    //发序列化
    Command cmm ;
    cmm.ParseFromString(aa) ;
    
    cout << "cmd:" << cmm.cmd() << endl ;
    cout << "key:" << cmm.key() << endl ;
    len = cmm.vals_size() ;
    for(int i=0; i<len; i++) {
        int a = cmm.vals(i).val_size() ;
        for(int j=0; j<a; j++) {
            cout << "value :" << cmm.vals(i).val(j) << endl;
        }
    }
    cout << "结束" << endl ;
    return 0;
}


