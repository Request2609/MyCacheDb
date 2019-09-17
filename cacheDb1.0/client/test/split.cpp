#include <iostream>
#include <vector>
using namespace std ;

vector<string> split(std::string oldStr, string exStr) {
    long len = oldStr.size() ;
    vector<string> ls ;
    string stmp = oldStr;
    int count = 0 ;
    while(1) {

        string tmp ="";
        int pos = stmp.find(exStr) ;
        if(pos == -1) {
            break ;
        }
        for(int i=0; i<pos; i++) {
            count ++ ;
            tmp+=stmp[i] ;
        }

        int j  = 0 ;
        for(j=pos; stmp[j] == ' ' || count>=len; j++, count++) ;
        stmp = stmp.c_str()+j;
        ls.push_back(tmp) ;
        if(count >= len) {
            break ;
        }
    }
    if(stmp.size() != 0) {
        ls.push_back(stmp) ;
    }
    return ls ;
}

int main() {
    string ss ;
    getline(cin, ss);
    vector<string>ll = split(ss, " ") ;
    int len = ll.size() ;
    cout << len << endl ;
    for(int i= 0; i<len; i++) {
        cout << ll[i] << endl ;
    }
    return 0;
}

