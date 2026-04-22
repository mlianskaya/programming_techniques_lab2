/**
 * @file BSTree.cpp
 * @brief Реализация бинарного дерева поиска.
 */

#include "BSTree.hpp"

BSTree::BSTree() : root(nullptr) {}

BSTree::~BSTree() {
    clear();
}

void BSTree::insert(const Applicant& a) {
    insert(root, a);
}

void BSTree::insert(BSTNode*& node, const Applicant& a) {
    if (node == nullptr) {
        node = new BSTNode(a.fullName, a);
        return;
    }
    if (a.fullName < node->key)
        insert(node->left, a);
    else if (a.fullName > node->key)
        insert(node->right, a);
    else
        node->values.push_back(a);
}

std::vector<Applicant> BSTree::find(const std::string& key) const {
    BSTNode* node = findNode(root, key);
    if (node) return node->values;
    return {};
}

BSTNode* BSTree::findNode(BSTNode* node, const std::string& key) const {
    if (node == nullptr || node->key == key) return node;
    if (key < node->key) return findNode(node->left, key);
    else return findNode(node->right, key);
}

void BSTree::clear() {
    clear(root);
    root = nullptr;
}

void BSTree::clear(BSTNode* node) {
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}