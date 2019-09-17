#include <iostream>
#include <regex>

using namespace std ;
/*
void isNum(const char* str) {
    //整数
    regex_t reg , reg1;
    const size_t nmatch = 1 ;
    regmatch_t pmatch[1] ;
    int cflags = REG_EXTENDED ;
    const char* Int = "^-?\\d+$" ;
    //浮点数
    const char* Float = "^(-?\\d+)(\\.\\d+)?$" ;
    //编译正则表达式
    regcomp(&reg,Int,cflags);
    regcomp(&reg1,Float,cflags);
    int status  = regexec(&reg, str, nmatch, pmatch, 0) ;
    if(status == REG_NOMATCH) {
        cout << "YES" << endl ;
        return  ;
    }
    status  = regexec(&reg1, str, nmatch, pmatch, 0) ;
    if(status == REG_NOMATCH) {
        cout << "YES" << endl ;
        return  ;
    }
    cout << "NO" << endl ;
}
*/
bool isNum(const char* num) {
    regex regFloat("^(-?\\d+)(\\.\\d+)?$");
    regex regInt("^-?\\d+$") ;
    bool res = regex_match(num, regFloat);
    if(res) {
        return res ;
    }
    res = regex_match(num, regInt);
    if(res) {
        return res ;
    }
    return false ;
}

int main() {
    
    const char* num = "hello" ;
    const char* floats = "12.09" ;
    const char* Int = "12" ;
    const char* t = "-12" ;
    const char* tt = "-12.09" ;
    const char * ttt = "122335457687877" ;
    cout << isNum(num) << endl ;
    cout << isNum(floats) << endl ;
    cout << isNum(Int) << endl ;
    cout << isNum(t) << endl ;
    cout << isNum(tt) << endl ;
    cout << isNum(ttt) << endl ;
    return 0;
}

