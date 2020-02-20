#include "dbObject.h"

//传进来的字符串中含有分数范围
//start end
std::vector<std::string> sortSet::getValues(const std::string s) {
    //解析分数范围
    int index = s.find(' ') ;
    std::string start = s.substr(0, index) ;
    int ss = atoi(start.c_str()) ;
    int len = s.size() ;
    std::string end = s.substr(index+1, len-index) ;
    int ee = atoi(end.c_str()) ;
    auto ls = rbt->getByScore(ss, ee) ;
    std::string value = "" ;
    std::vector<std::string>res ;
    for(auto tmp : ls) {
        for(auto tmp1 : tmp.second) {
            res.push_back(tmp1) ;
        }
    }
    return res ;
}


//intset集合中传值第一个从参数是分数，第二个参数是value
void sortSet::setValue(std::string value, ...) {
    if(rbt == nullptr) {
        rbt = std::make_shared<rb_tree>() ;
    }
    std::string score = value ;
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
    std::cout << "hash对象键值" << std::endl ;
    for(auto s : values) {
        std::cout << s.first << "   " <<s.second << std::endl ;
    }
}
//测试用
void strings :: print() {
    std::cout << type<< "    " << num<< "   "<< key << "    "<< value << std::endl ;
}

void lsObject :: print() {
    for(auto s : ls) {
        std::cout << s << "    " << std::endl ; 
    }
}

std::vector<std::string> hashSet :: getValues(const std::string s) {
    std::vector<std::string>ls ;
    auto val = values.find(s) ;
    if(val == values.end()) {
        ls.push_back("hashset has no this feild!") ;
        return ls ;
    }
    ls.push_back(values[s]) ;
    return ls ;
}

std::string hashSet :: getValue() {
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
    std::string vv = iter->first + "\r\n" + iter->second ;
    iter++ ;
    return vv ;
}

//字符串的形式传进来
void hashSet :: setValue(std::string value, ...) {
    std::string key = value ;
    va_list va ;
    va_start(va, value) ;
    char* val = va_arg(va, char*) ;
    values[key] = val ;
    va_end(va) ;
}


void lsObject :: setValue(std::string value, ...) {
    ls.push_back(value) ;
}   

void setContain :: setValue(std::string value, ...) {
    ls.insert(value) ;
}   


std::string setContain :: getValue() {
    std::string sss = "";
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

setContain::setContain() {

}

setContain::~setContain() {

}
std::string lsObject :: getValue() {
    std::string sss = "";
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

std::vector<std::string> lsObject :: getValues(std::string ss) {
    std::vector<std::string> tmp ;
    for(auto s : ls) {
        tmp.push_back(s) ;
    }
    return tmp ;
}
