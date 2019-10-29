#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>
#include <cmath>
#include <string>
using namespace std ;

class Solution {
public:
        //统计
        int getZero(long& n, string&s) {
                int flag = 0 ;
                while(n) {
                        if(n %10 != 0 || flag == 1) {
                                flag = 1 ;
                                int a =  n%10 ;
                                s+=to_string(a) ;
                                n = n/10 ;
                                continue ;
                        }
                        if(flag == 0) {
                                //末尾是０就直接将其去除
                                n /=10 ;  
                        }
                }
                return  1;
        }
        int reverse(int x) {
                string s ;
               int flag = 0 ;
               long aa = x  ;
                if(x < 0) {
                        aa = -aa ;
                        getZero(aa, s) ;
                        if(x <= INT32_MIN) {
                                return 0 ;
                        }
                        long a = -1*atol(s.c_str()) ;
                        if(a < INT32_MIN) {
                                return 0 ;
                        }
                        int as = a ;
                        return  as ;
                }
                else {
                        getZero(aa, s) ;
                }
                long ss = atoll(s.c_str()) ;
                if(ss >INT32_MAX) {
                        return 0;
                }
                int as = ss ;
                return as ;
    }
};

int main() {
        int n ;
        cin >> n ;
        Solution ss ;
        cout << ss.reverse(n)  << endl ;
}