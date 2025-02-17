#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <queue>
#include <stdexcept> // Para std::invalid_argument

template <typename T>
struct KeyAddressPair {
    T key;
    size_t address;
};

template <typename T>
class BTree {
private:
    struct Node {
        std::vector<KeyAddressPair<T>> keys;
        std::vector<Node*> children;
        bool leaf;

        Node(bool isLeaf = true) : leaf(isLeaf) {}

        // Implementação do destrutor inline
        ~Node() {
            if (!leaf) {
                for (auto child : children) {
                    delete child;
                }
            }
        }
    };

    Node* root;
    int t; // Ordem da árvore

    // Métodos auxiliares privados (DECLARAÇÕES)
    void splitChild(Node* parent, int index);
    void insertNonFull(Node* node, const KeyAddressPair<T>& keyAddress);
    bool searchKey(Node* node, const T& key) const;
    KeyAddressPair<T> getPred(Node* node, int index);
    KeyAddressPair<T> getSucc(Node* node, int index);
    void fill(Node* node, int index);
    void borrowFromPrev(Node* node, int index);
    void borrowFromNext(Node* node, int index);
    void merge(Node* node, int index);
    bool removeFromNode(Node* node, const T& key);
    void removeFromLeaf(Node* node, int index);
    void removeFromNonLeaf(Node* node, int index);
    void printTree(Node* node, int level, std::ostream& os) const;
    int getHeight(Node* node) const;
    void destroyNode(Node* node);
    int findKey(Node* node, const T& key) const;

public:
    BTree(int order);
    ~BTree();

    void insert(const KeyAddressPair<T>& keyAddress);
    bool remove(const T& key);
    bool search(const T& key) const;
    void print(std::ostream& os = std::cout) const;
};

// IMPLEMENTAÇÕES DOS MÉTODOS DA CLASSE TEMPLATE (DENTRO DO .h)

template <typename T>
BTree<T>::BTree(int order) : root(nullptr), t(order) {
    if (order < 2) {
        throw std::invalid_argument("Ordem da árvore deve ser maior ou igual a 2");
    }
}

template <typename T>
BTree<T>::~BTree() {
    destroyNode(root);
}

template <typename T>
void BTree<T>::splitChild(Node* parent, int index) {
    Node* child = parent->children[index];
    Node* newChild = new Node(child->leaf);

    for (int j = 0; j < t - 1; j++) {
        newChild->keys.push_back(child->keys[t + j]);
    }
    if (!child->leaf) {
        for (int j = 0; j < t; j++) {
            newChild->children.push_back(child->children[t + j]);
        }
        child->children.resize(t);
    }
    child->keys.resize(t - 1);

    parent->keys.insert(parent->keys.begin() + index, child->keys[t - 1]);
    parent->children.insert(parent->children.begin() + index + 1, newChild);
}

template <typename T>
void BTree<T>::insert(const KeyAddressPair<T>& keyAddress) {
    if (root == nullptr) {
        root = new Node(true);
        root->keys.push_back(keyAddress);
        return;
    }

    if (root->keys.size() == 2 * t - 1) {
        Node* newRoot = new Node(false);
        newRoot->children.push_back(root);
        splitChild(newRoot, 0);
        root = newRoot;
    }

    insertNonFull(root, keyAddress);
}

template <typename T>
bool BTree<T>::remove(const T& key) {

    if (!root) return false;

    bool result = removeFromNode(root, key);

    if (root->keys.empty()) {
        Node* oldRoot = root;
        root = root->leaf ? nullptr : root->children[0];
        delete oldRoot; // Limpeza de memória
    }

    return result;
}

template <typename T>
void BTree<T>::insertNonFull(Node* node, const KeyAddressPair<T>& keyAddress) {
    int i = node->keys.size() - 1;

    if (node->leaf) {
        node->keys.push_back(KeyAddressPair<T>{});
        while (i >= 0 && keyAddress.key < node->keys[i].key) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = keyAddress;
    } else {
        while (i >= 0 && keyAddress.key < node->keys[i].key) {
            i--;
        }
        i++;

        if (node->children[i]->keys.size() == 2 * t - 1) {
            splitChild(node, i);
            if (keyAddress.key > node->keys[i].key) {
                i++;
            }
        }
        insertNonFull(node->children[i], keyAddress);
    }
}

template <typename T>
bool BTree<T>::search(const T& key) const {
    return root && searchKey(root, key);
}

template <typename T>
bool BTree<T>::searchKey(Node* node, const T& key) const {
    int i = 0;
    while (i < node->keys.size() && key > node->keys[i].key) {
        i++;
    }

    if (i < node->keys.size() && key == node->keys[i].key) {
        return true;
    }

    if (node->leaf) {
        return false;
    }

    return searchKey(node->children[i], key);
}

template <typename T>
void BTree<T>::print(std::ostream& os) const {
    if (!root) {
        os << "Árvore vazia" << std::endl;
        return;
    }
    printTree(root, 0, os);
}

template <typename T>
void BTree<T>::printTree(Node* node, int level, std::ostream& os) const {
    if (node) {
        os << std::string(level * 4, ' ');
        os << "[";
        for (size_t i = 0; i < node->keys.size(); ++i) {
            os << node->keys[i].key;
            if (i < node->keys.size() - 1) os << ", ";
        }
        os << "]" << std::endl;

        if (!node->leaf) {
            for (Node* child : node->children) {
                printTree(child, level + 1, os);
            }
        }
    }
}

template <typename T>
void BTree<T>::merge(Node* node, int idx) {
    Node* child = node->children[idx];
    Node* sibling = node->children[idx + 1];

    child->keys.push_back(node->keys[idx]);

    for (int i = 0; i < sibling->keys.size(); ++i) {
        child->keys.push_back(sibling->keys[i]);
    }

    if (!child->leaf) {
        for (int i = 0; i < sibling->children.size(); ++i) {
            child->children.push_back(sibling->children[i]);
        }
    }

    node->keys.erase(node->keys.begin() + idx);
    node->children.erase(node->children.begin() + idx + 1);

    delete sibling;
}

template <typename T>
KeyAddressPair<T> BTree<T>::getPred(Node* node, int index) {
    Node* current = node->children[index];
    while (!current->leaf) {
        current = current->children[current->keys.size()];
    }
    return current->keys[current->keys.size() - 1];
}

template <typename T>
KeyAddressPair<T> BTree<T>::getSucc(Node* node, int index) {
    Node* current = node->children[index + 1];
    while (!current->leaf) {
        current = current->children[0];
    }
    return current->keys[0];
}

template <typename T>
void BTree<T>::fill(Node* node, int index) {
    if (index != 0 && node->children[index - 1]->keys.size() >= t) {
        borrowFromPrev(node, index);
    } else if (index != node->keys.size() && node->children[index + 1]->keys.size() >= t) {
        borrowFromNext(node, index);
    } else {
        if (index != node->keys.size()) {
            merge(node, index);
        } else {
            merge(node, index - 1);
        }
    }
}

template <typename T>
void BTree<T>::borrowFromPrev(Node* node, int index) {
    Node* child = node->children[index];
    Node* sibling = node->children[index - 1];

    for (int i = child->keys.size() - 1; i >= 0; --i) {
        child->keys[i + 1] = child->keys[i];
    }

    if (!child->leaf) {
        for (int i = child->children.size() - 1; i >= 0; --i) {
            child->children[i + 1] = child->children[i];
        }
    }

    child->keys[0] = node->keys[index - 1];
    node->keys[index - 1] = sibling->keys[sibling->keys.size() - 1];

    if (!child->leaf) {
        child->children[0] = sibling->children[sibling->children.size() - 1];
        sibling->children.pop_back();
    }

    sibling->keys.pop_back();
}

template <typename T>
void BTree<T>::borrowFromNext(Node* node, int index) {
    Node* child = node->children[index];
    Node* sibling = node->children[index + 1];

    child->keys.push_back(node->keys[index]);
    node->keys[index] = sibling->keys[0];

    if (!child->leaf) {
        child->children.push_back(sibling->children[0]);
        sibling->children.erase(sibling->children.begin());
    }

    sibling->keys.erase(sibling->keys.begin());
}

template <typename T>
bool BTree<T>::removeFromNode(Node* node, const T& key) {
    int index = findKey(node, key);

    if (index < node->keys.size() && node->keys[index].key == key) {
        if (node->leaf) {
            removeFromLeaf(node, index);
        } else {
            removeFromNonLeaf(node, index);
        }
        return true;
    }

    if (node->leaf) {
        return false;
    }

    bool lastChild = (index == node->keys.size());

    if (node->children[index]->keys.size() < t) {
        fill(node, index);
    }

    if (lastChild && index > node->keys.size()) {
        return removeFromNode(node->children[index - 1], key);
    } else {
        return removeFromNode(node->children[index], key);
    }
}

template <typename T>
void BTree<T>::removeFromLeaf(Node* node, int index) {
    node->keys.erase(node->keys.begin() + index);
}

template <typename T>
void BTree<T>::removeFromNonLeaf(Node* node, int index) {
    KeyAddressPair<T> key = node->keys[index];

    if (node->children[index]->keys.size() >= t) {
        KeyAddressPair<T> pred = getPred(node, index);
        node->keys[index] = pred;
        removeFromNode(node->children[index], pred.key);
    } else if (node->children[index + 1]->keys.size() >= t) {
        KeyAddressPair<T> succ = getSucc(node, index);
        node->keys[index] = succ;
        removeFromNode(node->children[index + 1], succ.key);
    } else {
        merge(node, index);
        removeFromNode(node->children[index], key.key);
    }
}

template <typename T>
int BTree<T>::getHeight(Node* node) const {
    if (!node) {
        return 0;
    }

    int height = 1;
    if (!node->leaf) {
        height += getHeight(node->children[0]);
    }
    return height;
}

template <typename T>
int BTree<T>::findKey(Node* node, const T& key) const {
    int index = 0;
    while (index < node->keys.size() && node->keys[index].key < key) {
        index++;
    }
    return index;
}

template<typename T>
void BTree<T>::destroyNode(Node* node) {
    if (node) {
        if (!node->leaf) {
            for (Node* child : node->children) {
                destroyNode(child);
            }
        }
        delete node;
    }
}
#endif //