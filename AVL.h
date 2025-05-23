#ifndef AVL_H
#define AVL_H
#include <iostream>
#include <string>
#include "AVL_Node.h"
#include "AVL_Iterator.h"

using namespace std;

template <typename T>
class AVLTree
{
public:
    typedef AVLIterator<T> iterator;

    iterator begin(AVLIterator<int>::Type _) {
        return iterator(root, _);
    }

    iterator end() {
        return iterator(nullptr);
    }

private:
    NodeAVL<T> *root;
public:
    AVLTree() : root(nullptr) {}

    void insert(T value) {
        insert(root, nullptr, value);
    }

    bool find(T value) {
        return find(root, value);
    }

    string getInOrder() {
        string result;
        inOrder(root, result);
        return result;
    }

    string getPreOrder() {
        string result;
        preOrder(root, result);
        return result;
    }

    string getPostOrder() {
        string result;
        postOrder(root, result);
        return result;
    }

    int height() {
        return root ? root->height : -1;
    }

    T minValue() {
        NodeAVL<T>* curr = root;
        if (!curr) throw runtime_error("Árbol vacío");
        while (curr->left) curr = curr->left;
        return curr->data;
    }

    T maxValue() {
        NodeAVL<T>* curr = root;
        if (!curr) throw runtime_error("Árbol vacío");
        while (curr->right) curr = curr->right;
        return curr->data;
    }

    bool isBalanced() {
        return isBalanced(root);
    }

    int size() {
        return size(root);
    }

    void remove(T value) {
        remove(root, value);
    }

    T successor(T value) {
        NodeAVL<T>* curr = root;
        NodeAVL<T>* succ = nullptr;
        while (curr) {
            if (value < curr->data) {
                succ = curr;
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
        if (!succ) throw runtime_error("No hay sucesor");
        return succ->data;
    }

    T predecessor(T value) {
        NodeAVL<T>* curr = root;
        NodeAVL<T>* pred = nullptr;
        while (curr) {
            if (value > curr->data) {
                pred = curr;
                curr = curr->right;
            } else {
                curr = curr->left;
            }
        }
        if (!pred) throw runtime_error("No hay predecesor");
        return pred->data;
    }

    void clear() {
        if (root) {
            root->killSelf();
            root = nullptr;
        }
    }

    void displayPretty() {
        displayPretty(root, "", true);
    }

    ~AVLTree() {
        clear();
    }

private:

    void insert(NodeAVL<T>*& node, NodeAVL<T>* parent, T value) {
        if (!node) {
            node = new NodeAVL<T>(value);
            node->parent = parent;
        } else if (value < node->data) {
            insert(node->left, node, value);
        } else if (value > node->data) {
            insert(node->right, node, value);
        } else {
            return;
        }
        updateHeight(node);
        balance(node);
    }

    bool find(NodeAVL<T>* node, T value) {
        while (node) {
            if (value == node->data) return true;
            node = value < node->data ? node->left : node->right;
        }
        return false;
    }

    int size(NodeAVL<T>* node) {
        if (!node) return 0;
        return 1 + size(node->left) + size(node->right);
    }

    bool isBalanced(NodeAVL<T>* node) {
        if (!node) return true;
        int bf = balancingFactor(node);
        return abs(bf) <= 1 && isBalanced(node->left) && isBalanced(node->right);
    }

    void remove(NodeAVL<T>*& node, T value) {
        if (!node) return;
        if (value < node->data) {
            remove(node->left, value);
        } else if (value > node->data) {
            remove(node->right, value);
        } else {
            if (!node->left || !node->right) {
                NodeAVL<T>* temp = node->left ? node->left : node->right;
                if (temp) temp->parent = node->parent;
                delete node;
                node = temp;
            } else {
                NodeAVL<T>* pred = node->maxLeft();
                node->data = pred->data;
                remove(node->left, pred->data);
            }
        }
        if (node) {
            updateHeight(node);
            balance(node);
        }
    }

    void inOrder(NodeAVL<T>* node, string& result) {
        if (!node) return;
        inOrder(node->left, result);
        result += to_string(node->data) + " ";
        inOrder(node->right, result);
    }

    void preOrder(NodeAVL<T>* node, string& result) {
        if (!node) return;
        result += to_string(node->data) + " ";
        preOrder(node->left, result);
        preOrder(node->right, result);
    }

    void postOrder(NodeAVL<T>* node, string& result) {
        if (!node) return;
        postOrder(node->left, result);
        postOrder(node->right, result);
        result += to_string(node->data) + " ";
    }

    void displayPretty(NodeAVL<T>* node, string prefix, bool isLeft) {
        if (node) {
            cout << prefix;
            cout << (isLeft ? "+--" : "\\--");
            cout << node->data << endl;
            displayPretty(node->left, prefix + (isLeft ? "|   " : "    "), true);
            displayPretty(node->right, prefix + (isLeft ? "|   " : "    "), false);
        }
    }


    int balancingFactor(NodeAVL<T>* node) {
        int hl = node->left ? node->left->height : -1;
        int hr = node->right ? node->right->height : -1;
        return hl - hr;
    }

    void updateHeight(NodeAVL<T>* node) {
        int hl = node->left ? node->left->height : -1;
        int hr = node->right ? node->right->height : -1;
        node->height = 1 + max(hl, hr);
    }

    void balance(NodeAVL<T>*& node) {
        int bf = balancingFactor(node);
        if (bf > 1) {
            if (balancingFactor(node->left) < 0)
                left_rota(node->left);
            right_rota(node);
        } else if (bf < -1) {
            if (balancingFactor(node->right) > 0)
                right_rota(node->right);
            left_rota(node);
        }
    }

    void left_rota(NodeAVL<T>*& x) {
        NodeAVL<T>* y = x->right;
        x->right = y->left;
        if (y->left) y->left->parent = x;
        y->left = x;
        y->parent = x->parent;
        x->parent = y;
        x = y;
        updateHeight(x->left);
        updateHeight(x);
    }

    void right_rota(NodeAVL<T>*& y) {
        NodeAVL<T>* x = y->left;
        y->left = x->right;
        if (x->right) x->right->parent = y;
        x->right = y;
        x->parent = y->parent;
        y->parent = x;
        y = x;
        updateHeight(y->right);
        updateHeight(y);
    }
};

#endif
