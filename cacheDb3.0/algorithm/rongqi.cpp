#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

bool comp(const pair<int, int>&t1, const pair<int, int>&t2) {
    return t1.second>t2.second ;
}

class Solution {
public:
    int maxArea(vector<int>& height) {
        sort(ls.begin(), ls.end(), comp) ;
        int max = -1 ;
        int index = 0 ;
        int tmp = 0 ;
                return max ;
    }
};

int main()
{
    return 0;
}

