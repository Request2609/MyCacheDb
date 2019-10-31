#include <iostream>
#include <unordered_map>
#include <unistd.h>
#include <sys/wait.h>
using namespace std ;

//创建记录aof的键值
class aofKey {
public :
 /*   bool operator==(const aofKey& a)const {
        return a.cmd==cmd&&a.num==num ;
    }*/
    long num ; //序列号
    string cmd ; //命令
} ;

class aofEqualFunc {
public :
    bool operator() (const aofKey& k1, const aofKey& k2) const{
        return k1.num == k2.num && k1.cmd == k2.cmd ;
    }
} ;

class aofHashFunc {
public :
    size_t operator() (const aofKey& k) const  {
        return hash<string>()(k.cmd)^hash<int>()(k.num) ;
    }
} ;


class aofOperation {
public:
    aofOperation() {}
    ~aofOperation() {}
public :
    unordered_map<aofKey, int, aofHashFunc, aofEqualFunc> ps ;
    void print() {
        for(auto res : ps) {
            cout << res.first.num << endl ;
            cout << res.second<< endl ;
        }
    }
};

int main()
{
    aofKey k ;
    k.num = 10 ;
    k.cmd = "helllo"  ;
    aofOperation ap ;
    ap.ps[k] = 10 ;
    ap.print() ;
    return 0;
}
