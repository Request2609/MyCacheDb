#pragma once
#include <list>
#include <fstream>
#include "redisDb.h"

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
    static std::shared_ptr<simpleLru>getSimpleLru() ;
    int size ;
    static std::shared_ptr<simpleLru>sl ;
    std::shared_ptr<searchTree>st ;
};

struct treeNode {
    treeNode(const key k) ;
    std::shared_ptr<key> k ;
    std::shared_ptr<treeNode>left ;
    std::shared_ptr<treeNode>right ;
};

class searchTree {
public :
    searchTree() {
    }
    ~searchTree() {
    }
    void insert(const key& k) ;
    std::shared_ptr<treeNode> deleteOne() ;
private:
    int size = 0 ;
    std::shared_ptr<treeNode>root ;
} ;
