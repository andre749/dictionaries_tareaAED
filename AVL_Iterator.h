#ifndef AVL_ITERATOR_H
#define AVL_ITERATOR_H

#include "AVL_Node.h"
#include <stack>
#include <queue>

template <typename T>
class AVLIterator {
public:
    enum Type {
        PreOrder, InOrder, PostOrder, BFS
    };

private:
    NodeAVL<T>* current;
    Type type;
    std::stack<NodeAVL<T>*> stk;      // para InOrder, PreOrder, PostOrder
    std::stack<NodeAVL<T>*> post_aux; // auxiliar para PostOrder
    std::queue<NodeAVL<T>*> q;        // para BFS

    void initInOrder(NodeAVL<T>* node) {
        current = node;
        while (current) {
            stk.push(current);
            current = current->left;
        }
        advanceInOrder();
    }

    void initPreOrder(NodeAVL<T>* node) {
        if (node) stk.push(node);
        advancePreOrder();
    }

    void initPostOrder(NodeAVL<T>* node) {
        if (!node) return;
        std::stack<NodeAVL<T>*> temp;
        temp.push(node);
        while (!temp.empty()) {
            NodeAVL<T>* n = temp.top(); temp.pop();
            post_aux.push(n);
            if (n->left) temp.push(n->left);
            if (n->right) temp.push(n->right);
        }
        advancePostOrder();
    }

    void initBFS(NodeAVL<T>* node) {
        if (node) q.push(node);
        advanceBFS();
    }

    void advanceInOrder() {
        if (stk.empty()) {
            current = nullptr;
            return;
        }
        current = stk.top(); stk.pop();
        NodeAVL<T>* temp = current->right;
        while (temp) {
            stk.push(temp);
            temp = temp->left;
        }
    }

    void advancePreOrder() {
        if (stk.empty()) {
            current = nullptr;
            return;
        }
        current = stk.top(); stk.pop();
        if (current->right) stk.push(current->right);
        if (current->left) stk.push(current->left);
    }

    void advancePostOrder() {
        if (post_aux.empty()) {
            current = nullptr;
            return;
        }
        current = post_aux.top(); post_aux.pop();
    }

    void advanceBFS() {
        if (q.empty()) {
            current = nullptr;
            return;
        }
        current = q.front(); q.pop();
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }

public:
    AVLIterator() : current(nullptr), type(InOrder) {}

    AVLIterator(NodeAVL<T>* root, Type type = InOrder) : type(type), current(nullptr) {
        switch (type) {
            case InOrder: initInOrder(root); break;
            case PreOrder: initPreOrder(root); break;
            case PostOrder: initPostOrder(root); break;
            case BFS: initBFS(root); break;
        }
    }

    AVLIterator<T>& operator=(AVLIterator<T> other) {
        current = other.current;
        type = other.type;
        stk = other.stk;
        post_aux = other.post_aux;
        q = other.q;

        return *this;
    }

    bool operator!=(AVLIterator<T> other) {
        return current != other.current;
    }

    AVLIterator<T>& operator++() {
        switch (type) {
            case InOrder: advanceInOrder(); break;
            case PreOrder: advancePreOrder(); break;
            case PostOrder: advancePostOrder(); break;
            case BFS: advanceBFS(); break;
        }
        return *this;
    }

    T operator*() {
        return current->data;
    }
};

#endif
