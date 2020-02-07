#include "lru.h"

shared_ptr<simpleLru>simpleLru::sl ;
const int FULL_SIZE = 10 ;

shared_ptr<simpleLru>simpleLru::getSimpleLru() {
    if(sl == nullptr) {
        sl = shared_ptr<simpleLru>(new simpleLru) ;
    }
    return sl ;
}

//统计lru
void simpleLru::statistic(const key& k) {
    if(sl == nullptr) {
        getSimpleLru() ;
        sl->init() ;
    }       
    sl->updateAndEliminate(k) ;
}

void simpleLru::init() {
    st = make_shared<searchTree>() ;
}

void simpleLru:: updateAndEliminate(const key&k) {
    key kk = k ;
    st->insert(k) ;
}

long simpleLru::getMem() {
    filebuf fb ;
    pid_t pid =getpid();
    string str = "/proc/"+to_string(pid) ;
    str+="/status" ;
    if(!fb.open(str.c_str(), ios::in)) {
        cout << __LINE__ <<"   " << __FILE__ << "  " << strerror(errno) << endl ;
        return -1 ;
    }   
    string ss ;
    istream is(&fb) ;
    while(getline(is, ss, '\n')) {
        if(ss.find("VmRSS") !=string::npos) {
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
void searchTree::insert(const key& k) {
    if(root == nullptr) {
        root = make_shared<treeNode>(k) ;
        root->left = nullptr ;
        root->right = nullptr ;
    }
    else {
        if(size == FULL_SIZE) {
            root = deleteOne() ;
        }
        auto ptr = make_shared<treeNode>(k) ;
        auto tmp = root ;
        shared_ptr<treeNode>pre ;
        while(tmp != nullptr) {
            if(*(ptr->k)==*(tmp->k)) {
                tmp->k->timer = ptr->k->timer ;
                break ;
            }
            else if(tmp->right != nullptr&&*(ptr->k) > *(tmp->k)){
                tmp = tmp->left ;
            }
            else if(tmp->left != nullptr&&*(ptr->k)<*(tmp->k)){
                tmp = tmp->right ;
            }
            else if(tmp->left == nullptr&&*(ptr->k)<*(tmp->k)) {
                ptr->left = tmp->left ;
                tmp->left = ptr->left ;
                break ;
            }
            else if(tmp->right == nullptr && *(ptr->k)>*(tmp->k)) {
                ptr->right = tmp->right ;
                tmp->right = ptr ;
                break ;
            }
        }
    }
}

treeNode::treeNode(const key k) {
    this->k = make_shared<key>(k) ;
}

shared_ptr<treeNode>searchTree::deleteOne() {
    auto tmp = root->left ;
    auto pre = root ;
    //删除一个根
    if(tmp == nullptr) {
        root = nullptr ;
        return  nullptr;
    }
    while(tmp) {
        pre = tmp ;
        tmp = tmp->right ;
    }
    pre->left = root->left ;
    pre->right = root->right ;
    pre->k = root->k ;
    return pre ;
}
    
