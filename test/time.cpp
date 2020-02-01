#include <iostream>
#include <fstream>
#include <map>
using namespace std ;

int main() {
    ifstream out  ;
    out.open("../config/save", ios::in) ;
    if(out.fail()) {
        cout << "打开文件出错！"<< endl ;
        return 1;
    }
    cout << "文件打开成功！"<< endl ;
    map<int, int>  ls ;
    string s ;
    while(!out.eof()) {
        int sec = 0, time=0 ;
        out >>s>>sec>>time ; 
        cout << "  " << s <<"  " << sec<< "  " << time<< endl ;
        ls.insert({sec, time}) ;
    }
    out.close() ;
    for(auto s : ls) {
        cout << s.first<<"      " << s.second << endl ;
    }
    return 0;
}

