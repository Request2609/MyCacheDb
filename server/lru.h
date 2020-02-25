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
    static int statistic(const key& k) ;
    static long getMem() ;
private:
    simpleLru():size(0) {}
    void init() ;
    int updateAndEliminate(const key& k) ;
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
    searchTree() ;
    ~searchTree() {
    }
    int insert(const key& k) ;
    void recover() ;
    static std::shared_ptr<key> getDelKey() ;
    void swap(std::shared_ptr<key>&key1, 
              std::shared_ptr<key>&key2) ;
private:
    bool isExist(const std::shared_ptr<treeNode>&tmp) ;
    int size = 0 ;
    //二叉堆
    std::vector<std::shared_ptr<treeNode>>heap ;
    static shared_ptr<key> delKey ;
    
} ;
