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

    BST(){
        root = NULL;
    }

    virtual Node * insert(VAL_TYPE val){

        Node *node = _search(root, val);
        if(node != NULL){
            return node;
        }

        node = new Node(val);
        Node *parent = _searchParent(root, val);
        if(parent == NULL){
            root = node;
        }else{
            if(node->val > parent->val){
                parent->right = node;
            }else{
                parent->left = node;
            }
            node->parent = parent;
        }

        return node;
    }

    virtual void remove(VAL_TYPE val){

        Node *node = search(val);
        if(node != NULL){
            _remove(node);
        }
    }

    Node *search(VAL_TYPE val){
        return _search(root, val);
    }

    void print(){

        cout << "[";
        _printInorder(root);
        cout << "]" << endl;
    }

    void _printInorder(Node *node){

        if(node != NULL){

            if(node->left != NULL){
                _printInorder(node->left);
            }

            cout << "," << node->val;

            if(node->right != NULL){
                _printInorder(node->right);
            }
        }
    }

    ~BST(){
        if(root != NULL){
            delete root;
        }
    }

protected:

    Node *_searchParent(Node *node, VAL_TYPE val){

        if(node == NULL){
            return NULL;
        }

        if(val > node->val){

            if(node->right != NULL){
                return _search(node->right, val);
            }
            return node;
        }

        if(node->left != NULL){
            return _search(node->left, val);
        }

        return node;
    }

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
            node1->right = node2->right;
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
            Node *temp = node1->parent;
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

        if(node == NULL){
            return ;
        }

        if(node->left == NULL && node->right == NULL){
            if(node->parent != NULL){
                if(node->parent->left == node){
                    node->parent->left = NULL;
                }else{
                    node->parent->right = NULL;
                }
            }

            delete node;

        }else if(node->left == NULL || node->right == NULL){

            Node *existingNode = NULL;
            if(node->left != NULL){
                existingNode = node->left;
            }else{
                existingNode = node->right;
            }

            if(node->parent != NULL){
                if(node->parent->left == node){
                    node->parent->left = existingNode;
                }else{
                    node->parent->right = existingNode;
                }
            }
            existingNode->parent = node->parent;

            delete node;

        }else{

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
    BST *bst = new BST();

    VAL_TYPE a[] = { 1, 10, 9, 15 };

    for(int I=0; I < 4; I++){
        cout << a[I] << endl;
        bst->insert(a[I]);
    }

    cout << bst->root->val << endl;

    bst->print();

    delete bst;
}
