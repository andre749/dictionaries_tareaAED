#ifndef AVL_NODE_H
#define AVL_NODE_H
#pragma once
template <typename T>
struct NodeAVL {
    T data;
    int height;
    NodeAVL<T>* left;
    NodeAVL<T>* right;
    NodeAVL<T>* parent;
    NodeAVL() : data(), left(nullptr), right(nullptr), parent(nullptr), height(0) {}
    NodeAVL(T value) : data(value), left(nullptr), right(nullptr), parent(nullptr), height(0) {}

    void killSelf(){
        if(left != nullptr) {
            left->killSelf();
            delete left;
            left = nullptr;
        }
        if(right != nullptr) {
            right->killSelf();
            delete right;
            right = nullptr;
        }
    }

    NodeAVL* maxLeft() {
        NodeAVL* curr = left;
        if (!curr) return nullptr;
        while (curr->right) curr = curr->right;
        return curr;
    }

    NodeAVL* minRight() {
        NodeAVL* curr = right;
        if (!curr) return nullptr;
        while (curr->left) curr = curr->left;
        return curr;
    }
};

#endif