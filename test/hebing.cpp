#include <iostream>
#include <vector>
using namespace std ;
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int index = 0 ;
        if(n == 0) {
            return ;
        }
        if(m == 0) {
            nums1 = nums2 ;
            return ;
        }

        for(int i=0; i<m; i++) {
            while(i<m&&nums2[index] >= nums1[i]) {
                i++ ;
            } 
            if(i<m) {
                swap(nums2[index], nums1[i]) ;
                //将数组2调整成有序数组
                for(int j=1; j<n; j++) {
                    if(nums2[j] < nums2[j-1]) {
                        swap(nums2[j], nums2[j-1]) ;
                    }
                    else {
                        break ;
                    }
                }
            }
        }
        for(int i=m; i<n+m&&index<n; i++) {
            nums1[i] = nums2[index] ;
            index++ ;
        }
    }
    void swap(int& num1, int& num2) {
        int tmp = move(num1) ;
        num1 = move(num2) ;
        num2 = move(tmp) ;
    }
};

int main()
{
    Solution ss ;
    vector<int>ls1(10, 0) ;
    vector<int>ls2(10, 0) ;
    int index1 = 0 ;
    while(1) {
        int a ;
        cin >> a ;
        if(a == -1) break ;
        ls1[index1] = a ;
        index1++ ;
    }
    int index2 = 0 ;
    while(1) {
        int b ;
        cin >> b ;
        if(b == -1) break ;
        ls2[index2] = b ;
        index2 ++ ;
    }
    ss.merge(ls1, index1, ls2, index2) ;
    int len = index1+index2 ;
    for(int i=0; i<len; i++) {
        cout << ls1[i] <<" " ;
    }
    cout << endl ;
    return 0;
}


