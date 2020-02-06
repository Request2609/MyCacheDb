#pragma once
#include <list>
#include "redisDb.h"
using namespace std ;

class simpleLru {
public:
    ~simpleLru() {}
    static void statistic(const key& k) ;
private:
    simpleLru():size(0) {}
    void updateAndEliminate(const key& k) ;
    static shared_ptr<simpleLru>getSimpleLru() ;
    int size ;
    list<key>kList ;
    static shared_ptr<simpleLru>sl ;
};

struct treeNode {
    key k ;
    shared_ptr<treeNode>left ;
    shared_ptr<treeNode>right ;
};

class searchTree {
public :
    searchTree() {
    }
    ~searchTree() {
    }
    void insert(const key& k) ;
private:
    void build() ;
    void deleteOne() ;
    int size = 0 ;
    shared_ptr<treeNode>root ;
} ;
