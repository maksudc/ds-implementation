#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <stack>
#include <queue>
#include <cmath>

using namespace std;


typedef int VAL_TYPE;

class Node{
public:

    VAL_TYPE val;

    Node *left;
    Node *right;
    Node *parent;

    Node(VAL_TYPE _val){
        val = _val;
        left = NULL;
        right = NULL;
        parent = NULL;
    }

    ~Node(){

        if(left != NULL){
            delete left;
        }

        if(right != NULL){
            delete right;
        }
    }
};

class BST{
public:

    Node *root;

    BST(Node *_root){
        root = root;
    }

    virtual void insert(VAL_TYPE val){

        if(){}
    }

    virtual void remove(VAL_TYPE val){

        Node *node = search(val);

        if(node != NULL){
            // find inorder successor
        }
    }

    Node *search(VAL_TYPE val){
        return _search(root, val);
    }

protected:
    Node _search(Node *node, VAL_TYPE val){

        if(node == NULL || node->val == val){
            return node;
        }

        if(val > node->val){
            return _search(node->left, val);
        }

        return _search(node->right, val);
    }
};

class AVLTree: public BST{
public:

    AVLTree(Node *root): BST(root){

    }
};

int main()
{
    Node *root = new Node();



    delete root;
}
