/**
 * @file BSTree.hpp
 * @brief Заголовочный файл бинарного дерева поиска (BST).
 * @details Реализована вставка и поиск всех элементов с заданным ключом.
 *          Ключом является поле fullName класса Applicant.
 */

#pragma once
#include <vector>
#include <string>
#include "Applicant.hpp"

/**
 * @struct BSTNode
 * @brief Узел бинарного дерева поиска.
 */
struct BSTNode {
    std::string key;                    ///< Ключ (ФИО)
    std::vector<Applicant> values;      ///< Все абитуриенты с этим ключом
    BSTNode* left;                      ///< Левый потомок
    BSTNode* right;                     ///< Правый потомок

    /**
     * @brief Конструктор узла.
     * @param k Ключ.
     * @param a Абитуриент (добавляется в вектор values).
     */
    BSTNode(const std::string& k, const Applicant& a)
        : key(k), left(nullptr), right(nullptr) {
        values.push_back(a);
    }
};

/**
 * @class BSTree
 * @brief Класс бинарного дерева поиска.
 */
class BSTree {
public:
    BSTree();
    ~BSTree();

    /**
     * @brief Вставка абитуриента в дерево.
     * @param a Объект Applicant.
     */
    void insert(const Applicant& a);

    /**
     * @brief Поиск всех абитуриентов по ключу (ФИО).
     * @param key Искомое ФИО.
     * @return Вектор копий найденных объектов (может быть пустым).
     */
    std::vector<Applicant> find(const std::string& key) const;

    /** @brief Очистка дерева. */
    void clear();

private:
    BSTNode* root; ///< Корень дерева

    void insert(BSTNode*& node, const Applicant& a);
    BSTNode* findNode(BSTNode* node, const std::string& key) const;
    void clear(BSTNode* node);
};