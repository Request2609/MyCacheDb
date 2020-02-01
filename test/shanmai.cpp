#include <iostream>
#include <vector>
using namespace std ;
class Solution {
public:
   int longestMountain(vector<int>& A){
        int len = A.size() ;
        int max = 0 ;
        int l = 1 ;
        int flag = 0 ;
        for(int i=0; i<len; i++) {
            while(i-l-1>=0&& i+l+1<len && A[i-l]>A[i-l-1] && A[i+l]>A[i+1+l]) {
                cout << A[i-l]<<" v " << A[i-l-1] << endl ;
                flag = 1 ;
                l++ ;
            }
            if(l>max) {
                max = l ;
                l= 0 ;
            }
        }
        if(flag == 0)return 0 ;
        return max*2+1 ;
    }       
};
int main() {
    vector<int>ls ;
    while(1) {
        int a ;
        cin >> a ;
        if(a == -1)break ;
        ls.push_back(a) ;
    }
    Solution ss ;
    cout << ss.longestMountain(ls) << endl ;
    return 0;
}

