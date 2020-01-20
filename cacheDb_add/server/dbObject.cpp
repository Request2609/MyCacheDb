#include "dbObject.h"

//测试用
void hashSet :: print() {
    cout << "hash对象键值" << endl ;
    for(auto s : values) {
        cout << s.first << "   " <<s.second << endl ;
    }
}
//测试用
void strings :: print() {
    cout << type<< "    " << num<< "   "<< key << "    "<< value << endl ;
}


vector<string> hashSet :: getValues(const string s) {
    vector<string>ls ;
    auto val = values.find(s) ;
    if(val == values.end()) {
        ls.push_back("hashset has no this feild!") ;
        return ls ;
    }
    ls.push_back(values[s]) ;
    return ls ;
}

string hashSet :: getValue() {
    static int flag = 0 ;
    static auto iter = values.begin() ;
    if(flag == 0) {
        iter = values.begin() ;
    }
    if(iter == values.end()) {
        iter = values.begin() ;
        flag = 0 ;
        return "" ;
    }
    flag = 1 ;
    string vv = iter->first + "\r\n" + iter->second ;
    iter++ ;
    return vv ;
}

//字符串的形式传进来
void hashSet :: setValue(string value, ...) {
    string key = value ;
    va_list va ;
    va_start(va, value) ;
    char* val = va_arg(va, char*) ;
    values[key] = val ;
    cout << key << "     " << val << endl ;
    va_end(va) ;
}


void lsObject :: setValue(string value, ...) {
    ls.push_back(value) ;
}   

string lsObject :: getValue() {
    string sss = "";
    if(ls.empty()) {
        return "" ;
    }
    for(auto s= ls.begin(); s != ls.end(); s++) {
        sss = *s ;
        ls.erase(s) ;
        break ;
    }   
    return sss ;
}

vector<string> lsObject :: getValues(string ss) {
    vector<string> tmp ;
    for(auto s : ls) {
        tmp.push_back(s) ;
    }
    return tmp ;
}
