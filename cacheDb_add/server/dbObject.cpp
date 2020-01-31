#include "dbObject.h"

//传进来的字符串中含有分数范围
//start end
vector<string> sortSet::getValues(const string s) {
    //解析分数范围
    int index = s.find(' ') ;
    string start = s.substr(0, index) ;
    int ss = atoi(start.c_str()) ;
    int len = s.size() ;
    string end = s.substr(index+1, len-index) ;
    int ee = atoi(end.c_str()) ;
    cout << ss << "  " << ee << endl ;
    cout <<"节点数量:" << rbt->getNum() << endl ;
    auto ls = rbt->getByScore(ss, ee) ;
    string value = "" ;
    vector<string>res ;
    for(auto tmp : ls) {
        for(auto tmp1 : tmp.second) {
            res.push_back(tmp1) ;
        }
    }
    return res ;
}

//intset集合中传值第一个从参数是分数，第二个参数是value
void sortSet::setValue(string value, ...) {
    if(rbt == nullptr) {
        rbt = make_shared<rb_tree>() ;
    }
    string score = value ;
    va_list va ;
    va_start(va, value) ;
    char* val = va_arg(va, char*) ;
    value = val ;
    value = val ;
    va_end(va) ;
    int s = atoi(score.c_str()) ;
    rbt->insert_tree(s, value) ;
}

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

void lsObject :: print() {
    cout << "list对象的键值:" << key << endl ;
    for(auto s : ls) {
        cout << s << "    " << endl ; 
    }
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
