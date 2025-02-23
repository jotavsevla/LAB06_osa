#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <queue>
#include <stdexcept>

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
        ~Node() {
            if (!leaf) {
                for (auto child : children) {
                    delete child;
                }
            }
        }
    };

    Node* root;
    size_t t; // Ordem da árvore (mínimo t chaves, máximo 2t chaves)

    // Métodos auxiliares privados
    void splitChild(Node* parent, size_t index) {
        Node* y = parent->children[index];
        Node* z = new Node(y->leaf);

        // Para um nó com n chaves, a mediana está em n/2
        size_t mid = y->keys.size() / 2;
        KeyAddressPair<T> medianKey = y->keys[mid];

        // Move a segunda metade das chaves para o novo nó
        for (size_t j = mid + 1; j < y->keys.size(); j++) {
            z->keys.push_back(y->keys[j]);
        }

        // Se não for folha, move os filhos correspondentes
        if (!y->leaf) {
            for (size_t j = mid + 1; j <= y->keys.size(); j++) {
                z->children.push_back(y->children[j]);
            }
        }

        // Redimensiona o nó original
        y->keys.resize(mid);
        if (!y->leaf) {
            y->children.resize(mid + 1);
        }

        // Insere a mediana no pai
        parent->keys.insert(parent->keys.begin() + index, medianKey);
        parent->children.insert(parent->children.begin() + index + 1, z);
    }

    void insertNonFull(Node* node, const KeyAddressPair<T>& keyAddress) {
        int i = node->keys.size() - 1;

        if (node->leaf) {
            // Encontra a posição para inserir a nova chave
            while (i >= 0 && keyAddress.key < node->keys[i].key) {
                i--;
            }
            // Insere a chave na posição correta
            node->keys.insert(node->keys.begin() + i + 1, keyAddress);
        } else {
            // Encontra o filho que deve receber a nova chave
            while (i >= 0 && keyAddress.key < node->keys[i].key) {
                i--;
            }
            i++;

            // Se o filho está cheio, faz o split antes de inserir
            if (node->children[i]->keys.size() >= 2 * t) {
                splitChild(node, i);
                if (keyAddress.key > node->keys[i].key) {
                    i++;
                }
            }
            insertNonFull(node->children[i], keyAddress);
        }
    }

    bool searchKey(Node* node, const T& key) const {
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

    void printTree(Node* node, int level, std::ostream& os) const {
        if (node) {
            os << std::string(level * 4, ' ') << "[";
            for (size_t i = 0; i < node->keys.size(); i++) {
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

    void destroyNode(Node* node) {
        if (node) {
            if (!node->leaf) {
                for (auto child : node->children) {
                    destroyNode(child);
                }
            }
            delete node;
        }
    }

public:
    BTree(size_t order) : root(nullptr), t(order) {
        if (order < 2) {
            throw std::invalid_argument("B-Tree order must be at least 2");
        }
    }

    ~BTree() {
        destroyNode(root);
    }

    void insert(const KeyAddressPair<T>& keyAddress) {
        if (root == nullptr) {
            root = new Node();
            root->keys.push_back(keyAddress);
            return;
        }

        // Se a raiz está cheia, cria nova raiz
        if (root->keys.size() >= t) {
            Node* newRoot = new Node(false);
            newRoot->children.push_back(root);
            root = newRoot;
            splitChild(root, 0);
        }

        insertNonFull(root, keyAddress);
    }

    bool search(const T& key) const {
        return root && searchKey(root, key);
    }

    void print(std::ostream& os = std::cout) const {
        if (!root) {
            os << "Árvore vazia" << std::endl;
            return;
        }
        printTree(root, 0, os);
    }
};

#endif // BTREE_H