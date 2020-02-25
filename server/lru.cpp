#include "lru.h"

std::shared_ptr<simpleLru>simpleLru::sl ;
std::shared_ptr<key> searchTree::delKey = nullptr;

const int FULL_SIZE = 10 ;

searchTree::searchTree() {
}

std::shared_ptr<simpleLru>simpleLru::getSimpleLru() {
    if(sl == nullptr) {
        sl = std::shared_ptr<simpleLru>(new simpleLru) ;
    }
    return sl ;
}

void searchTree::swap(std::shared_ptr<key>&key1, 
          std::shared_ptr<key>&key2) {
    auto tmp = std::move(key1) ;
    key1 = std::move(key2) ;
    key2 = std::move(tmp) ;
}
//统计lru
int simpleLru::statistic(const key& k) {
    if(sl == nullptr) {
        getSimpleLru() ;
        sl->init() ;
    }       
    return sl->updateAndEliminate(k) ;
}

void simpleLru::init() {
    st = std::make_shared<searchTree>() ;
}

int simpleLru:: updateAndEliminate(const key&k) {
    key kk = k ;
    return st->insert(k) ;
}

long simpleLru::getMem() {
    std::filebuf fb ;
    pid_t pid =getpid();
    std::string str = "/proc/"+std::to_string(pid) ;
    str+="/status" ;
    if(!fb.open(str.c_str(), ios::in)) {
        std::string s = "         " +std::to_string(__LINE__) +__FILE__+strerror(errno) ;
        aofRecord::log(s) ;
        return -1 ;
    }   
    std::string ss ;
    std::istream is(&fb) ;
    while(getline(is, ss, '\n')) {
        if(ss.find("VmRSS") !=std::string::npos) {
            int index = ss.find("VmRSS") ;
            ss = ss.data()+index ;
            int end = ss.find("kB") ;
            end = end-1 ;
            ss = ss.substr(0, end-index) ;
            break ;
        }   
    }   
    fb.close() ;
    return atoi(ss.c_str()) ; 
}

//插入一个元素
int searchTree::insert(const key& k) {
    auto tmp = std::make_shared<treeNode>(k) ;
    if(heap.empty()) {
        tmp->left = nullptr ;
        tmp->right = nullptr ;
        heap.push_back(k) ;
        return 0;
    }
    //当前堆的大小
    int size = heap.size() ;
    //堆满了，就删除堆顶元素，然后重新调整堆
    if(size == FULL_SIZE) {
        if(*(heap[0]->key) > k) {
            delKey = tmp->k ;
            return 1 ;
        }
        else {
            delKey = heap[0] ;
            heap[0] = tmp ;
            recover()  ;     
            return 0 ;
        }   
    }
    if(isExist(tmp)){
        return 0 ;
    }
    //这个键不存在将键存到堆中
    heap.push_back(tmp) ;
    //恢复堆结构
    recover() ;
}

//查看这个键是否已经存在树中，要是存的，就将时间更新
bool searchTree::isExist(const std::shared_ptr<treeNode>&node) {
    for(int i=0; i<heap.size(); i++) {
        if(*(node->k) == *(heap[i]->k)) {
            heap[i]->k->timer = node->k->timer ;
            return true ;
        }
    }
    return false ;
}

void searchTree::recover() {
    int index=  0 ;
    int left = 0 ;
    int right = 0 ;
    int len = heap.size() ;
    auto tmp = heap[0] ;
    while(right<len) {
        left = index*2+1 ;
        right = index*2+2 ;
        if(right<len) {
            if(*(heap[left]->k)<=*(heap[left]->k)) {
                index = left ;
            }           
            else {
                index = right ;
            }
            //同样直接返回
            if(*(tmp->k)==*(heap[index]->k)) {
                return ;
            }
            if(*(tmp->k)<*(heap[index]->k)) {
                swap(tmp, heap[index]) ;
            }
            tmp = heap[index] ;
            index++ ;
            continue ;
        }
        //只有左下标小于size，是最后一个节点
        if(left<size) {
            if(*(tmp->k)<*(heap[left]->k)) {
                swap(tmp, heap[left]) ;
            }
            break ;
        }
    }
}

treeNode::treeNode(const key k) {
    this->k = std::make_shared<key>(k) ;
}

std::shared_ptr<key> searchTree::getDelKey() {
    return delKey ;
}

