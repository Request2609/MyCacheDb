#include <iostream>
#include <string>
#include <vector>
using namespace std ;

class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if(strs.size() == 0) {
            return "" ;
        } 
        if(strs.size() == 1) {
            return strs[0] ;
        }
        int j =  0;
        while(1) {
            for(int i=0; i<strs.size(); i++) {
                if(j >= strs[i].size()) {
                    return strs[i].substr(0, j) ;
                }
                char c = strs.at(0).at(j) ;
                if(c != strs[i].at(j)) {
                    return strs[i].substr(0, j) ;
                }
            }
        }
        j++ ;
    } 
} ;
int main()
{
    std::cout << "Hello world" << std::endl;
    return 0;
}

