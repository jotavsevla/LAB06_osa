#include "BTree.h"
#include <iostream>
using namespace std;


int main() {
    // Cria uma árvore B de ordem 3 (você pode ajustar a ordem conforme necessário)
    BTree<int> arvore(3);

    // Insere alguns elementos na árvore
    arvore.insert({10, 100}); // Chave 10, Endereço 100
    arvore.insert({20, 200}); // Chave 20, Endereço 200
    arvore.insert({5, 50});   // Chave 5, Endereço 50
    arvore.insert({15, 150}); // Chave 15, Endereço 150
    arvore.insert({25, 250}); // Chave 25, Endereço 250>
    arvore.insert({30, 300}); // Chave 30, Endereço 300
    arvore.insert({1, 10});   // Chave 1, Endereço 10
    arvore.insert({7, 70});   // Chave 7, Endereço 70
    arvore.insert({22, 220}); // Chave 22, Endereço 220
    arvore.insert({12, 120}); // Chave 12, Endereço 120
    arvore.insert({18, 180}); // Chave 18, Endereço 180
    arvore.insert({28, 280}); // Chave 28, Endereço 280
    arvore.insert({35, 350}); // Chave 35, Endereço 350
    arvore.insert({3, 30});   // Chave 3, Endereço 30
    arvore.insert({8, 80});   // Chave 8, Endereço 80
    arvore.insert({16, 160}); // Chave 16, Endereço 160
    arvore.insert({21, 210}); // Chave 21, Endereço 210
    arvore.insert({23, 230}); // Chave 23, Endereço 230
    arvore.insert({27, 270}); // Chave 27, Endereço 270
    arvore.insert({32, 320}); // Chave 32, Endereço 320
    arvore.insert({38, 380}); // Chave 38, Endereço 380
    arvore.insert({2, 20});   // Chave 2, Endereço 20
    arvore.insert({9, 90});   // Chave 9, Endereço 90
    arvore.insert({14, 140}); // Chave 14, Endereço 140
    arvore.insert({17, 170}); // Chave 17, Endereço 170
    arvore.insert({19, 190}); // Chave 19, Endereço 190
    arvore.insert({24, 240}); // Chave 24, Endereço 240
    arvore.insert({26, 260}); // Chave 26, Endereço 260
    arvore.insert({29, 290}); // Chave 29, Endereço 290
    arvore.insert({31, 310}); // Chave 31, Endereço 310
    arvore.insert({33, 330}); // Chave 33, Endereço 330
    arvore.insert({36, 360}); // Chave 36, Endereço 360
    arvore.insert({39, 390}); // Chave 39, Endereço 390
    arvore.insert({4, 40});   // Chave 4, Endereço 40
    arvore.insert({6, 60});   // Chave 6, Endereço 60
    arvore.insert({11, 110}); // Chave 11, Endereço 110
    arvore.insert({13, 130}); // Chave 13, Endereço 130
    arvore.insert({40, 400}); // Chave 40, Endereço 400

    // Imprime a árvore
    std::cout << "Árvore B:\n";
    arvore.print();

    // Pesquisa alguns elementos
    std::cout << "\nPesquisa:\n";
    std::cout << "5 está na árvore? " << arvore.search(5) << std::endl;
    std::cout << "15 está na árvore? " << arvore.search(15) << std::endl;
    std::cout << "40 está na árvore? " << arvore.search(40) << std::endl;
    std::cout << "42 está na árvore? " << arvore.search(42) << std::endl;

    // Remove alguns elementos
    std::cout << "\nRemoção:\n";
    arvore.remove(15);
    arvore.remove(25);

    // Imprime a árvore após a remoção
    std::cout << "\nÁrvore B após remoção:\n";
    arvore.print();

    return 0;
}