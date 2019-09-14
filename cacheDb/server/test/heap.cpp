#include <iostream>
#include <vector>
#include <memory>
#include <sys/time.h>
#include <functional>

using namespace std  ;

class clientInfo {
    typedef function<void()> callBack ;
public :
    //设置间隔时间
    void setmInterval(unsigned int times) { mInterval = times ; }
    //用户注册定时事件时计算一次超时时间
    void setExpire(unsigned long long m) { end = m ;}
    //设置用户回调函数
    void setCb(callBack cb) { call = move(cb) ; }
    //执行回调函数
    void cb() { call() ; }
    unsigned long getEnd() { return end ; }
private :
    callBack call ;
    //间隔时间
    unsigned long long mInterval ;
    //结束时间
    unsigned long long end ;
} ;

class heap {
public :
    int swap(shared_ptr<clientInfo>& s, shared_ptr<clientInfo>& t) ;
    int getParent(int num) ;
    //向堆中添加用户信息
    int heapAppend(shared_ptr<clientInfo>client) ;
    void print() ;
    int del() ;
    void reset() ;
    void start() ;
    //获取堆顶元素
    shared_ptr<clientInfo> getTop() ;
private :
    //管理用户的信息节点
    vector<shared_ptr<clientInfo>>ls ;
} ;
//获取当前时间
class timer {
public :
    static unsigned long long getCurTime() ;
} ;

//获取堆定元素
shared_ptr<clientInfo> heap :: getTop() {
    return ls[0] ;
}

//遍历一遍时间堆将过期时间处理掉
void heap :: start() {
    
}

unsigned long long  timer :: getCurTime() {
#ifdef _MSC_VER 
    _timeb timeBuffer ;
    _timeb timebuffer;
    _ftime(&timebuffer);
    unsigned long long ret = timebuffer.time;
    ret = ret * 1000 + timebuffer.millitm;
    return ret ;
#else 
    timeval tv ;
    gettimeofday(&tv, 0);
    unsigned long long ret = tv.tv_sec;
    return ret * 1000 + tv.tv_usec / 1000 ;
#endif
}

int heap :: swap(shared_ptr<clientInfo>& s, shared_ptr<clientInfo>& t) {
    shared_ptr<clientInfo> tmp = s ;
    s = t ;
    t = tmp ;
    return 1 ;
}

//最小堆父亲节点总是比孩子节点的值要小一点
//向堆中添加元素，将要添加的元素放置在最后
//根据父亲和孩子的关系，查询父亲节点，要是父亲节点的
//值比孩子节点的值大，则将父亲节点和孩子节点进行互换---->这是最小堆的上浮操作
int heap :: heapAppend(shared_ptr<clientInfo> client) {
    
    //往末未插入元素
    int index = 0 ;
    ls.push_back(client) ;
    int len = ls.size() ;
    //总共有１条数据
    if(len == 1) {
        return 0 ;
    }
    int tmp = ls.size()-1 ;
    //上浮
    while(tmp) {
        //获取父亲节点的下标
        index = getParent(tmp) ;
        //最小堆父亲节点要大于孩子节点的数据
        if(ls[index] > ls[tmp]) {
            swap(ls[index], ls[tmp]) ;
        }
        tmp = index ;
    }
    //这里要保证左孩子小于右孩子的值
    reset() ;
    return 1 ;
}

//这个函数主要是保证堆中的左孩子比右孩子要小
void heap :: reset() {
    int s = 0 ;
    int len = ls.size() ;
    while(1) {
        //做孩子和有孩子下标
        int l = s*2+1 ;
        int r = s*2+2 ;
        if(r >= len) {
            break ;
        }
        if(ls[l] > ls[r]) {
            swap(ls[l], ls[r]) ;
        }
        s++ ;
    }
}


int heap :: getParent(int tmp) {
    //获取父亲节点的下标
    if(tmp%2 == 1) {
        return  tmp/2 ;
    } 
    else {
        return (tmp/2-1) ;
    }
}

//删除堆顶元素
int heap :: del() {
    if(ls.size() == 0) {
        return -1 ;
    }
    //将第一个元素删除
    int len = ls.size() ;
    //记录原来对顶元素的值
    shared_ptr<clientInfo> rec = ls[0] ;
    //将最后一个元素的放在堆顶
    ls[0] = ls[len-1] ;
    //删除最后一个元素
    vector<clientInfo>::reverse_iterator iter = ls.rbegin() ;
    //删除最后一个元素
    vector<int>::reverse_iterator(ls.erase((++iter).base())) ;
    int s = 0 ;
    //从第一个元素开始进行下沉
    while(1) {
        int r = s*2+2 ;
        int l = s*2+1 ;
        if(r >= len) {
            break ;
        }
        //先和做孩子比较,要是比左孩子大
        if(ls[s] > ls[l]) {
            swap(ls[s], ls[l]) ; 
            //将index设置成左孩子的下标
            s = l ;
        }
        else {
            //退出
            break ;
        }
    }
    reset() ;
    return rec ;
}

void heap ::  print() {
    //打印终止时间
    for(shared_ptr<clientInfo> i : ls) {
        cout << i->getEnd() << "  " ;
    }
    cout << endl ;
}

int main() {
    heap h ;
    while(1) {
        int a ;
        cin >> a ;
        if(a == -1) {
            break ;
        }
        h.heapAppend(a) ;
    }
    
    //打印当前堆中的元素
    cout << "没删除元素" << endl ;
    h.print() ;
    cout << "删除一个元素" << endl ;
    h.del() ;
    h.print() ;
    cout << "删除两个元素" << endl ;
    h.del() ;
    h.print() ;
    return 0;
}

