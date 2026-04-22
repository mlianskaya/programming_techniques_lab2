/**
 * @file RBTree.hpp
 * @brief Заголовочный файл красно-черного дерева (RBTree).
 * @details Реализована вставка и поиск с балансировкой.
 *          Цвета: RED, BLACK.
 */

#pragma once
#include <vector>
#include <string>
#include "Applicant.hpp"

/** @brief Цвет узла. */
enum Color { RED, BLACK };

/**
 * @struct RBNode
 * @brief Узел красно-черного дерева.
 */
struct RBNode {
    std::string key;                    ///< Ключ
    std::vector<Applicant> values;      ///< Значения
    RBNode* left;                       ///< Левый потомок
    RBNode* right;                      ///< Правый потомок
    RBNode* parent;                     ///< Родитель
    Color color;                        ///< Цвет

    /**
     * @brief Конструктор узла.
     * @param k Ключ.
     * @param a Абитуриент.
     */
    RBNode(const std::string& k, const Applicant& a)
        : key(k), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
        values.push_back(a);
    }
};

/**
 * @class RBTree
 * @brief Класс красно-черного дерева.
 */
class RBTree {
public:
    RBTree();
    ~RBTree();

    void insert(const Applicant& a);
    std::vector<Applicant> find(const std::string& key) const;
    void clear();

private:
    RBNode* root;

    void rotateLeft(RBNode* x);
    void rotateRight(RBNode* x);
    void fixInsert(RBNode* z);
    RBNode* findNode(RBNode* node, const std::string& key) const;
    void clear(RBNode* node);
};