#include <iostream>
using namespace std ;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

void build(TreeNode** root) {
    int a ;
    cin >> a ;
    if(a != -1) {
        *root=(TreeNode*)malloc(sizeof(TreeNode)) ;
        (*root)->val = a ;
        build(&((*root)->left)) ;
        build(&(*root)->right);
    }
}

void destroy(TreeNode* root) {
    if(root) {
        destroy(root->left) ;
        destroy(root->right) ;
        free(root) ;
        root = NULL ;
    }   
}

void print(TreeNode* root) {
    if(root) {
        print(root->left) ;
        cout << root->val << endl ;
        print(root->right) ;
    }
}


class Solution {
public:
    int sum = 0 ;
    TreeNode* convertBST(TreeNode* root) {
        static TreeNode * r = root ;
        if(root) {
            convertBST(root->right) ;
            sum+=root->val ;
            root->val = sum ;
            convertBST(root->left) ;
        } 
        return r ;
    }
};

int main()
{
    TreeNode*root =NULL ;
    build(&root) ;
    print(root) ;
    Solution ss ;
    ss.convertBST(root) ;
    print(root) ;
    destroy(root) ;
    return 0;
}

