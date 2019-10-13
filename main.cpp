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

    int height;

    Node(VAL_TYPE _val){
        val = _val;
        left = NULL;
        right = NULL;
        parent = NULL;

        height = 0;
    }

    bool isBalanced(){
        return (int)abs(getLeftHeight() - getRightHeight()) <= 1;
    }

    int getLeftHeight(){
        if(left != NULL){
            return left->height;
        }
        return -1;
    }

    int getRightHeight(){
        if(right != NULL){
            return right->height;
        }
        return -1;
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

// Needs templating support for attaching any class definition
class BST{

protected:
    Node *root;

public:
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

    Node *avlInsert(VAL_TYPE val){


        Node *node = insert(val);
        Node *parent = _searchParent(root, val);

        if(parent != NULL){

            Node *current = parent;

            while(current != NULL){

                if(!current->isBalanced()){
                    balance(current);
                }
                current->height = max(current->left, current->right) + 1;
                // Setting height of each node after insertion
                current = current->parent;
            }
        }
    }

    void balance(Node *x){

        Node *p = x->parent;

        Node *y = NULL;

        if(x->getLeftHeight() > x->getRightHeight()){
            y = x->left;
        }else{
            y = x->right;
        }

        Node *z = NULL;

        if(y->getLeftHeight() > y->getRightHeight()){
            z = y->left;
        }else{
            z = y->right;
        }

        if(x->left == y && y->left == z){

            rotateRight(x);

            x->height = max(x->getLeftHeight(), x->getRightHeight()) + 1;
            z->height = max(z->getLeftHeight(), z->getRightHeight()) + 1;
            y->height = max(y->getLeftHeight(), y->getRightHeight()) + 1;

        }else if(x->left == y && y->right == z){

            rotateLeft(y);
            rotateRight(x);

            x->height = max(x->getLeftHeight(), x->getRightHeight()) + 1;
            y->height = max(y->getLeftHeight(), y->getRightHeight()) + 1;
            z->height = max(z->getLeftHeight(), z->getRightHeight()) + 1;

        }else if(x->right == y && y->right == z){

            rotateLeft(x);

            x->height = max(x->getLeftHeight(), x->getRightHeight()) + 1;
            z->height = max(z->getLeftHeight(), z->getRightHeight()) + 1;
            y->height = max(y->getLeftHeight(), y->getRightHeight()) + 1;

        }else if(x->right == y && y->left == z){

            rotateRight(y);
            rotateLeft(x);

            x->height = max(x->getLeftHeight(), x->getRightHeight()) + 1;
            y->height = max(y->getLeftHeight(), y->getRightHeight()) + 1;
            z->height = max(z->getLeftHeight(), z->getRightHeight()) + 1;
        }
    }

    void parentLinkReplace(Node *node, Node *replacement){

        Node *rnode = NULL;

        if(node->parent != replacement){
            rnode = replacement;
        }

        if(node->parent != NULL){
            if(node->parent->left == node){
                node->parent->left = rnode;
            }else{
                node->parent->right = rnode;
            }
        }
    }

    void rotateRight(Node *node){

        Node *x = node;
        Node *y = node->left;

        Node *T1 = y->left;
        Node *T2 = y->right;
        Node *T3 = x->right;

        y->right = x;
        x->left = T2;

        parentLinkReplace(x, y);
        x->parent = y;p
    }

    void rotateLeft(Node *node){

        Node *x = node;
        Node *y = node->right;

        Node *T1 = y->left;
        Node *T2 = y->right;
        Node *T3 = x->right;

        Node *p = x->parent;

        y->left = x;
        x->right = T1;

        parentLinkReplace(x, y);
        x->parent = y;
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
                Node *parent = _searchParent(node->right, val);
            }
            return node;
        }

        if(node->left != NULL){
            return _searchParent(node->left, val);
        }

        return node;
    }

    Node *_search(Node *node, VAL_TYPE val){

        if(node == NULL || node->val == val){
            return node;
        }

        if(val > node->val){
            return _search(node->right, val);
        }

        return _search(node->left, val);
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
            if(temp != node2){
                node2->left = temp;
            }

            temp = node1->right;
            node1->right = node2->right;
            if(temp != node2){
                node2->right = temp;
            }
        }

        if(node1 != NULL && node1->parent != NULL){
            if(node1->parent->left == node1){
                node1->parent->left = node2;
            }else{
                node1->parent->right = node2;
            }
        }

        if(node2 != NULL && node2->parent != NULL){

            if(node2->parent != node1){
                if(node2->parent->left == node2){
                    node2->parent->left = node1;
                }else{
                    node2->parent->right = node1;
                }
            }
        }

        if(node1 != NULL && node2 != NULL){

            Node *temp = node1->parent;
            if(node2->parent != node1){
                node1->parent = node2->parent;
            }else{
                node1->parent = NULL;
            }
            node2->parent = temp;

            if(node2->parent == NULL){
                root = node2;
            }
        }
    }

    void _clean(Node *node){

        if(node != NULL){
            node->parent = NULL;
            node->left = NULL;
            node->right = NULL;
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

            _clean(node);
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

            _clean(node);
            delete node;

        }else{

            Node *successor = _findInorderSuccessor(node);
            _swap(node, successor);
            _remove(node);
        }
    }
};

int main()
{
    BST *bst = new BST();

    VAL_TYPE a[] = { 5, 10, 3, 1, 9, 15 };

    for(int I=0; I < 6; I++){
        cout << a[I] << endl;
        bst->insert(a[I]);
    }

    bst->print();

    bst->remove(1);

    bst->print();

    delete bst;
}
