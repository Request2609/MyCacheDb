#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "msg.pb.h"
using namespace std ;
using namespace Messages ;


int  serializeToString(Command cmd, vector<string>ls, string* s) {  
     int ret = 0 ;
      //第一个是命令
      //第二个是key
      //第三个是value
      cout << ls.size() << endl ;
      int len = ls.size() ;
      if(len < 3) {
          return -1 ;
      }
      //第一个是命令
      cmd.set_len(10) ;
      cmd.set_cmd(ls[0]) ;
      //第二个是key
      cmd.set_key(ls[1]) ;
      //设置值
      Value* val = cmd.add_vals() ;
      //设置值,值可能重复
      cout <<"数组数据：" << endl ;
  cout <<   cmd.cmd() << endl ;
   cout <<  cmd.key() << endl ;
      for(int i=2; i<len; i++) {
          string* a = val->add_val() ;
          *a = ls[i] ;
          cout << *a << endl ;
      }
      //将设置好的结果序列化成string
      cmd.SerializeToString(s) ;
      return 1 ;
}

Command parseString(string& aa) {
    Command cmm ;
    cmm.ParseFromString(aa) ;
    return cmm ;
}
