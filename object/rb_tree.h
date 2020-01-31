#pragma once
#include <iostream>
#include <memory>
#include <set>
#include <map>
#include <queue>

using namespace std ;
//附加颜色没有用处
const int INVALID = 0 ;
//节点是黑色
const int BLACK = 1 ;
//节点是红色
const int  RED = 2 ;
//节点是复合颜色
const int ALL = 3 ;

class rb_tree ;
class tree_node ;
class rb_tree ;

typedef shared_ptr<tree_node> NODE ;

class tree_node {
    friend class rb_tree;
public :
    tree_node():
        data(-1),
        own_color(RED),
        original_color(INVALID), 
        root(nullptr),
        left(nullptr),
        right(nullptr),
        parent(nullptr) 
    {}
    ~tree_node() {}
    set<string>getValue() {
        return value ;
    }
    void setScore(int sc) {
        data = sc ;
    }
    void addValue(string s) {
        value.insert(s) ;
    }
private :
    int data ;
    int own_color ;
    //对应分数所存储的字符串
    set<string>value ;
    int original_color ;
    NODE root ;
    NODE left ;
    NODE right ;
    NODE parent ;
    int flag ;
};

class rb_tree {
public:
    rb_tree():root(nullptr) {}
    ~rb_tree() {}
public :
    map<int, set<string>> getByScore(int start, int end) ;
    void travelAllTree(NODE root, vector<NODE>&) ;
    void insert_tree(int info, const string& s) ;
    NODE find_node(int key) ;
    void delete_node(int num) ;
    void print_rb_tree() ;
private :
    NODE get_minimum(NODE cur) ;
    //右旋
    void rb_transform(NODE tmp1, NODE tmp2) ;
    NODE get_delete_node(int num) ;
    void right_rotate(NODE cur) ;
    void left_rotate(NODE cur) ;
    void fix_up_insert(NODE cur) ;
    void insert_by_bst_way(NODE cur) ;
    void fix_delete_tree(NODE cur) ;
private :
    NODE root ;
    NODE null ;
};
