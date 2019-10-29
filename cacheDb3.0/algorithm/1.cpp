#include <iostream>
#include <stack>
#include <vector>
using namespace std ;

class Solution {
public:
    void getNum(int n, vector<int>& ls) {
        if(n == 0) {
            ls.push_back(0) ;
            return ;
        }
        while(n) {
                int a = n%10 ;
                ls.push_back(a) ;
                n = n/10 ;
        }   
    }
    bool  isPalindrome(int x) {
        vector<int>ls ;   
        if(x < 0) {
            return false ;
        }
        getNum(x, ls) ;
        int start = 0 ;
        int end = ls.size() -1;
        while(start != end) {
            if(ls[start] != ls[end]) {
                    return false ;
            }       
            start ++ ;
            if(start == end) {
                break ;                    
            }
            end-- ;
        }
        return true ;
    }
};

int main() {
    int n ;
    cin >> n ;
    Solution ss ;
    int res = ss.isPalindrome(n) ;
    cout << "结果:"<< res << endl ;
}