#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std ;

int main()
{
    vector<int> ls ;
    while(1)  {
        int a ;
        cin >> a ;
        if(a == -1) {
            break ;
        }   
        ls.push_back(a) ;
    }
    if(find(ls.begin(), ls.end(), 4) != ls.begin()) {
        cout << "找到" << endl ;
    }
    else {
        cout << "没找到的！" << endl;
    }

    return 0;
}

