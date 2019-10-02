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

        //if(){}
    }

    virtual void remove(VAL_TYPE val){

        Node *node = search(val);
        _remove(node);
    }

    Node *search(VAL_TYPE val){
        return _search(root, val);
    }

protected:
    Node *_search(Node *node, VAL_TYPE val){

        if(node == NULL || node->val == val){
            return node;
        }

        if(val > node->val){
            return _search(node->left, val);
        }

        return _search(node->right, val);
    }

    Node *_findInorderSuccessor(Node *node){

        if(node == NULL || node->left== NULL && node->right == NULL){
            return NULL;
        }

        Node *current = node->left;
        while(current != NULL && current->right != NULL){
            current = current->right;
        }

        if(current == NULL){
            current = node->right;
            while(current != NULL && current->left != NULL){
                current = current->left;
            }
        }

        return current;
    }

    void _swap(Node *node1, Node *node2){

        if(node1 != NULL && node2 != NULL){
            Node *temp = node1->left;
            node1->left = node2->left;
            node2->left = temp;

            temp = node1->right;
            node1->right = temp;
            node2->right = temp;
        }

        if(node1 != NULL && node1->parent != NULL){
            if(node1->parent->left == node1){
                node1->parent->left = node2;
            }else{
                node1->parent->right = node2;
            }
        }

        if(node2 != NULL && node2->parent != NULL){
            if(node2->parent->left == node2){
                node2->parent->left = node1;
            }else{
                node2->parent->right = node1;
            }
        }

        if(node1 != NULL && node2 != NULL){
            temp = node1->parent;
            node1->parent = node2->parent;
            node2->parent = temp;

            if(node1->parent == NULL){
                root = node1;
            }else if(node2->parent == NULL){
                root = node2;
            }
        }
    }

    void _remove(Node *node){

        if(node != NULL){
            // find inorder successor
            Node *successor = _findInorderSuccessor(node);
            _swap(node, successor);
            _remove(node);
        }
    }
};

class AVLTree: public BST{
public:

    AVLTree(Node *root): BST(root){

    }
};

int main()
{
    Node *root = new Node(0);



    delete root;
}
