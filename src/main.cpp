#include "BTree.h"
#include <iostream>
using namespace std;

int main() {
    BTree<int> tree(3); // ordem 3: mín 3 chaves, máx 6 chaves

    // Inserir alguns valores de teste
    for(int i = 1; i <= 20; i++) {
        tree.insert({i, static_cast<size_t>(i*100)});
    }

    std::cout << "Árvore B:\n";
    tree.print();

    return 0;
}