#include "dbObject.h"

void hashSet :: print() {
    for(auto s : values) {
        cout << num <<"==============>  " <<  s.first <<"  -------  "<< s.second <<"    " <<  type<< endl ;
    }
}

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
    va_end(va) ;
}

void lsObject :: setValue(string key, ...) {
    string value = key ;
    this->key = value ;
    va_list va ;
    va_start(va, key) ;
    //将参数列表加到队列
    while(1) {
        char* val = va_arg(va, char*) ;
        if(strcmp(val, "") == 0) {
            break ; 
        }
        ls.push_back(val) ;
    }
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
