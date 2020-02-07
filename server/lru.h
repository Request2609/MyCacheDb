#pragma once
#include <list>
#include <fstream>
#include "redisDb.h"
using namespace std ;

class searchTree ;
struct treeNode ;
class key ;
class simpleLru {
public:
    ~simpleLru() {}
    static void statistic(const key& k) ;
    static long getMem() ;
private:
    simpleLru():size(0) {}
    void init() ;
    void updateAndEliminate(const key& k) ;
    static shared_ptr<simpleLru>getSimpleLru() ;
    int size ;
    static shared_ptr<simpleLru>sl ;
    shared_ptr<searchTree>st ;
};

struct treeNode {
    treeNode(const key k) ;
    shared_ptr<key> k ;
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
    shared_ptr<treeNode> deleteOne() ;
private:
    int size = 0 ;
    shared_ptr<treeNode>root ;
} ;
