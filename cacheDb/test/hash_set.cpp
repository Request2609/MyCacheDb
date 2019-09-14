#include <iostream>
#include <hash_set>
using namespace std ;
using namespace __gnu_cxx;
int main()
{
    const int n = 2 ;
    int s1[4] = {1,2,3,3} ;
    int s2[3] = {5,6,7} ;
    hash_set<int>sa(s1,s1+4) ;//声明一个集合，元素维数组s1
    hash_set<int>sa1(s2, s2+3) ;
    ostream_iterator<int>output(cout, "") ;
    cout << "输出集合sa的元素：" ;
    copy(sa.begin(), sa.end(), output) ;
    cout << "sa1的元素:" ;
    copy(sa1.begin(), sa1.end(), output) ;
    return 0;
}

