#include "sortSet.h"

//传进来的字符串中含有分数范围
//start end
vector<string> sortSet::getValues(const string s) {
    //解析分数范围
    int index = s.find(' ') ;
    string start = s.substr(index) ;
    int ss = atoi(start.c_str()) ;
    int len = s.size() ;
    string end = s.substr(index+1, len-index) ;
    int ee = atoi(end.c_str()) ;
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
    va_end(va) ;
    int s = atoi(score.c_str()) ;
    rbt->insert_tree(s, value) ;
}

