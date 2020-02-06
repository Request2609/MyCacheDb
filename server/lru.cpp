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
    }       

}

void simpleLru:: updateAndEliminate(const key&k) {
    key kk = k ;
    list<key>::iterator iter = ;
    for(auto s=kList.begin(); s!=kList.end(); s++) {
        iter =s ;
        if(s < k) {
            kList->insert(s, kk);         
            break ;
        }   
    }
}

void searchTree::insert(const key& k) {
    if(root == nullptr)
        root = make_shared<treeNode>(k) ;
    else {
        if(size == FULL_SIZE) {
            deleteOne() ;
        }
        auto ptr = make_shared<key>(k) ;
        auto tmp = root ;
        while(tmp != nullptr) {
            if(*ptr>)
        }
    }
}

void deleteOne() {

}
