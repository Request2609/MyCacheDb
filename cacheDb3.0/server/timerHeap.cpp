#define _CRT_SECURE_NO_WARNINGS
#include "timerHeap.h"

#ifdef _MSC_VER
# include <sys/timeb.h>
#else
# include <sys/time.h>
#endif
 

map<int, shared_ptr<aeEvent>>* MyTimer :: data ;
shared_ptr<aeEpoll> MyTimer :: aep ;
// Timer
MyTimer::MyTimer(shared_ptr<TimerManager>& manager)
	: manager_(manager)
	, m_nHeapIndex(-1) {
}
 
MyTimer::~MyTimer() {
}

int TimerManager :: getSize() {
    return heap_.size() ;
}

void  MyTimer :: start(Func fun, unsigned int interval, TimerType timetpe) {
	m_nInterval = interval;
	m_timerfunc = fun;
	m_nExpires = interval + TimerManager::get_current_millisecs();
	manager_->add_timer(*this);
	timerType_= timetpe;
}
 
void MyTimer :: on_timer(unsigned long long now) {
    //执行回调函数
    m_timerfunc(*MyTimer :: data, fd, MyTimer :: aep) ;
}

//客户端时间没到，执行此操作
void MyTimer :: add_time(unsigned long long now) {
    //先删除当前的节点
    manager_->remove_timer(m_nHeapIndex) ;
    //给当前节点延长时间
    m_nExpires = m_nInterval + now ;
    //添加到堆中
    manager_->add_timer(*this) ;
}

// TimerManager
void TimerManager::add_timer(MyTimer timer) {
	//插到数组最后一个位置上，上浮
	timer.m_nHeapIndex = heap_.size();
	HeapEntry entry = { timer.m_nExpires, shared_ptr<MyTimer>(new MyTimer(timer))};
	heap_.push_back(entry);
    //调整队列的元素位置,保持二叉搜索树的特性
	up_heap(heap_.size() - 1);
}
 
void TimerManager:: removeAll() {
    heap_.clear() ;
}

void TimerManager::remove_timer(int index) {
	//头元素用数组未元素替换，然后下沉
	if (!heap_.empty() && index < heap_.size()) {
		if (index == heap_.size() - 1) {
			heap_.pop_back();
		}
		else {
			swap_heap(index, heap_.size() - 1);
			heap_.pop_back();
			size_t parent = (index - 1) / 2;
			if (index > 0 && heap_[index].time < heap_[parent].time)
				up_heap(index);
			else
				down_heap(index);
		}
	}
}
 
void TimerManager :: detect_timers(int fd) {
    cout << "当前监听事件的个数:" << heap_.size() << endl ;
    for(auto res : heap_) {
        unsigned long long now = get_current_millisecs();
        if(res.timer->getFd() == fd) {
            if((res.time-now) < 300) {
                res.timer->add_time(now) ;
            }
        }
    }
}
    
void TimerManager :: printTime(long now) {
    for(auto res : heap_) {
        cout <<"客户端超时时间"<< res.time <<"       现在时间：" << now << endl ; 
    } 
}

//侦测时间超时是timermanager管理
void TimerManager::detect_timers() {
    unsigned long long now = get_current_millisecs();
    vector<int> ls ;
    //时间到
    while (!heap_.empty()) {
        //超时的话就调用相应的函数，并将对象一处堆
        shared_ptr<MyTimer> timer = heap_[0].timer ;
        if(heap_[0].time <= now) {
            remove_timer(timer->m_nHeapIndex);
            timer->on_timer(now);
        }
        else {
            timer->add_time(now) ;   
        }
    }
}


void TimerManager::up_heap(size_t index) {
	//下至上，和父节点比较。如果小于父节点上浮
	size_t parent = (index - 1) / 2;
	while (index > 0 && heap_[index].time < heap_[parent].time) {
		swap_heap(index, parent);
		index = parent;
		parent = (index - 1) / 2;
	}
}
 
void TimerManager::down_heap(size_t index) {
	//从上到下，算出左右子节点，和最小的交换
	size_t lchild = index * 2 + 1;
	while (lchild < heap_.size()) {
		size_t minChild = (lchild + 1 == heap_.size() || heap_[lchild].time < heap_[lchild + 1].time) ? lchild : lchild + 1;
		if (heap_[index].time < heap_[minChild].time)
			break;
		swap_heap(index, minChild);
		index = minChild;
		lchild = index * 2 + 1;
	}
}
 
void TimerManager::swap_heap(size_t index1, size_t index2) {
	HeapEntry tmp = heap_[index1];
	heap_[index1] = heap_[index2];
	heap_[index2] = tmp;
	heap_[index1].timer->m_nHeapIndex = index1;
	heap_[index2].timer->m_nHeapIndex = index2;
}
 
unsigned long long TimerManager::get_current_millisecs() {
#ifdef _MSC_VER
	_timeb timebuffer;
	_ftime(&timebuffer);
	unsigned long long ret = timebuffer.time;
	ret = ret * 1000 + timebuffer.millitm;
	return ret;
#else
	timeval tv;
	::gettimeofday(&tv, 0);
	unsigned long long ret = tv.tv_sec;
	return ret * 1000 + tv.tv_usec / 1000;
#endif
}
