/**
 * @file RBTree.cpp
 * @brief Реализация красно-черного дерева.
 * @details Содержит реализацию методов класса RBTree:
 *          вставка с балансировкой, поиск, очистка.
 *          Используется для хранения объектов Applicant по ключу fullName.
 */

#include "RBTree.hpp"
#include <algorithm>

RBTree::RBTree() : root(nullptr) {}
/**
 * @brief Конструктор по умолчанию.
 *        Инициализирует пустое дерево (корень = nullptr).
 */

RBTree::~RBTree() {
    clear();
}
/**
 * @brief Деструктор.
 *        Освобождает всю память, занятую узлами дерева.
 */

/**
 * @brief Левый поворот вокруг узла n.
 * @param n Узел, вокруг которого выполняется поворот.
 * 
 * 1. pivot = правый потомок n.
 * 2. pivot становится на место n.
 * 3. Левый потомок pivot становится правым потомком n.
 * 4. n становится левым потомком pivot.
 */
void RBTree::rotateLeft(RBNode* n) {
    RBNode* pivot = n->right;
    if (!pivot) return;

    // pivot занимает место n в дереве
    pivot->parent = n->parent;
    if (n->parent != nullptr) {
        if (n->parent->left == n)
            n->parent->left = pivot;
        else
            n->parent->right = pivot;
    } else {
        root = pivot; // n был корнем
    }

    // левый потомок pivot становится правым потомком n
    n->right = pivot->left;
    if (pivot->left != nullptr)
        pivot->left->parent = n;

    // n становится левым потомком pivot
    n->parent = pivot;
    pivot->left = n;
}

/**
 * @brief Правый поворот вокруг узла n.
 * @param n Узел, вокруг которого выполняется поворот.
 * 
 * Симметричен левому повороту:
 * 1. pivot = левый потомок n.
 * 2. pivot занимает место n.
 * 3. Правый потомок pivot становится левым потомком n.
 * 4. n становится правым потомком pivot.
 */
void RBTree::rotateRight(RBNode* n) {
    RBNode* pivot = n->left;
    if (!pivot) return;

    pivot->parent = n->parent;
    if (n->parent != nullptr) {
        if (n->parent->left == n)
            n->parent->left = pivot;
        else
            n->parent->right = pivot;
    } else {
        root = pivot;
    }

    n->left = pivot->right;
    if (pivot->right != nullptr)
        pivot->right->parent = n;

    n->parent = pivot;
    pivot->right = n;
}

void RBTree::fixInsert(RBNode* z) {
    /**
     * @brief Восстанавливает свойства красно-черного дерева после вставки красного узла.
     * @param z Вставленный красный узел.
     * @details Выполняет перекрашивания и повороты до тех пор,
     *          пока не будут выполнены все свойства дерева.
     *          В конце корень всегда делается черным.
     */
    while (z->parent && z->parent->color == RED) {
        // Случай: родитель z — левый потомок деда
        if (z->parent == z->parent->parent->left) {
            RBNode* y = z->parent->parent->right; // дядя
            if (y && y->color == RED) {
                // Случай 1: дядя красный → перекрашивание
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent; // поднимаемся к деду
            } else {
                // Случай 2: дядя черный
                if (z == z->parent->right) {
                    // дополнительный левый поворот
                    z = z->parent;
                    rotateLeft(z);
                }
                // затем правый поворот и перекраска
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateRight(z->parent->parent);
            }
        } else {
            // Симметричный случай: родитель z — правый потомок деда
            RBNode* y = z->parent->parent->left; // дядя
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateLeft(z->parent->parent);
            }
        }
    }
    root->color = BLACK; // корень всегда черный
}


void RBTree::insert(const Applicant& a) {
    /**
     * @brief Вставка нового абитуриента в дерево.
     * @param a Объект Applicant.
     * @details Сначала выполняется обычный поиск места для вставки.
     *          Если ключ (ФИО) уже существует, объект добавляется
     *          в вектор values существующего узла.
     *          Иначе создается новый красный узел, который затем
     *          балансируется вызовом fixInsert().
     */
    RBNode* z = new RBNode(a.fullName, a);
    RBNode* y = nullptr;
    RBNode* x = root;

    // Поиск места вставки
    while (x) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else if (z->key > x->key)
            x = x->right;
        else {
            // Ключ уже существует — добавляем значение и удаляем z
            x->values.push_back(a);
            delete z;
            return;
        }
    }

    // Прикрепляем новый узел к родителю
    z->parent = y;
    if (!y)
        root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;

    // Восстанавливаем свойства красно-черного дерева
    fixInsert(z);
}

std::vector<Applicant> RBTree::find(const std::string& key) const {
    /**
     * @brief Поиск всех абитуриентов по ключу (ФИО).
     * @param key Искомое ФИО.
     * @return Вектор копий найденных объектов.
     *         Если ключ не найден, возвращается пустой вектор.
     */
    RBNode* node = findNode(root, key);
    if (node) return node->values;
    return {};
}

RBNode* RBTree::findNode(RBNode* node, const std::string& key) const {
    /**
     * @brief Внутренний вспомогательный метод поиска узла по ключу.
     * @param node Узел, с которого начинается поиск.
     * @param key Искомый ключ.
     * @return Указатель на узел с заданным ключом или nullptr, если не найден.
     */
    while (node) {
        if (key < node->key)
            node = node->left;
        else if (key > node->key)
            node = node->right;
        else
            return node;
    }
    return nullptr;
}

void RBTree::clear() {
    /**
     * @brief Публичный метод очистки дерева.
     *        Удаляет все узлы и устанавливает корень в nullptr.
     */
    clear(root);
    root = nullptr;
}

void RBTree::clear(RBNode* node) {
    /**
     * @brief Рекурсивная очистка поддерева.
     * @param node Корень удаляемого поддерева.
     * @details Сначала рекурсивно удаляются левое и правое поддеревья,
     *          затем удаляется сам узел.
     */
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}