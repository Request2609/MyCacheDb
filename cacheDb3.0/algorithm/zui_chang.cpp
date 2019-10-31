#include <iostream>
#include <string>
#include <vector>
using namespace std ;
   vector<int>ls ;
        int len = strs.size() ;
        int index = 0 ;
        int l = 0 ;
        for(int i=0; i<len; i++) {
            if(l<strs[i].size()) {
                l = strs[i].size() ;
                index = i ;
            }
            ls.push_back(strs[i].size()) ;
        }
        //将最短的串拿出来
        string s = strs[index] ;
        for(int i=0; i<len; i++) {
            if(i == index) continue ;
            while(strs[i].find(s) == string::npos) {
                s = s.substr(0, s.length()-1) ;
            }
            if(s.empty()) {
                return "" ;
            }
        }
        return s ;
    }   
};
int main()
{
    vector<string>ls ;
    while(1) {
        string a ;
        cin >> a ;
        if(a == "1") {
            break ;
        }
        ls.push_back(a) ;
    }
    Solution ss ;
    cout <<ss.longestCommonPrefix(ls) << endl ;
    return 0;
}

