#include <iostream>
#include <vector>

using namespace std ;
/*
//暴力求解
class Solution {
public:
    int maxArea(vector<int>& height) {
        int m = -1 ;
        int len = height.size() ;
        int index = 0 ;
        int fir = 0 ;
        int tmp = 0 ;
        vector<pair<int, int>>ls ;       
        for(int i=0; i<len; i++) {
            ls.push_back({i, height[i]}) ;
        }
        while(index < len) {
            for(int i=index+1; i<len; i++) {
                fir =ls[i].first ;
                tmp = min(ls[i].second, ls[index].second)*abs(ls[index].first-ls[i].first) ;
                if(tmp > m) {
                    m = tmp ;
                }
            }
            index ++ ;
        }
        cout << m << endl ;
        return m ;
    }
};
*/
//双指针
class Solution {
public :
    int maxArea(vector<int>&height) {
        int left = 0 ;
        int right = height.size()-1 ;
        int m = -1 ;
        int tmp = 0 ;
        while(1) {
            tmp = min(height[left],height[right])*abs(right-left) ;
            if(tmp > m) {
                m = tmp ;
            } 
            if(left == right) {
                break ;
            }   
            if(height[left]<height[right]) {
                left++ ;
            }
            else {
                right-- ;
            }
        }
        return m ;
    }      
} ;
//
int main()
{
    vector<int>ls ;
    while(1) {
        int a ;
        cin >> a ;
        if(a == -1) break ;
        ls.push_back(a) ;
    }
    Solution ss ;
    cout << ss.maxArea(ls) << endl ;
    return 0;
}

