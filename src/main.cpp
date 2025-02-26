#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include "BTree.h"
#include "BTreeFileManager.h"
#include "Book.h"

using namespace std;

#include <iostream>
#include <vector>
#include <string>
#include "BTree.h"
#include "BTreeFileManager.h"
#include "IntegerRecord.h"

using namespace std;

int main() {
    // Cria um gerenciador para armazenar IntegerRecord
    // O primeiro parâmetro é o tipo da chave (int)
    // O segundo parâmetro é o tipo do registro (IntegerRecord)
    BTreeFileManager<int, IntegerRecord> manager("integers.dat", "integers_index.dat");

    cout << "=== Sistema de Armazenamento com Árvore B para Inteiros ===" << endl;

    // Gera uma sequência de inteiros para demonstrar a estrutura
    vector<int> values;

    // Caso 1: Sequência ordenada (força divisões pelo lado direito)
    cout << "\n[Teste 1] Inserindo valores em sequência ordenada:" << endl;
    for (int i = 10; i <= 100; i += 10) {
        values.push_back(i);
    }

    // Insere os valores no arquivo e constrói o índice
    for (int val : values) {
        IntegerRecord record{val};  // O valor armazenado é igual ao seu índice
        manager.insertRecord(record, val);
        cout << "Inserido: chave=" << val << ", valor=" << record.value << endl;
    }

    // Exibe a estrutura da árvore B
    cout << "\nEstrutura da árvore B após inserções sequenciais:" << endl;
    manager.printIndexStructure();

    // Teste de recuperação
    cout << "\n[Teste 2] Recuperando registros por chave:" << endl;
    for (int key : {20, 50, 80}) {
        try {
            IntegerRecord record = manager.getRecordByKey(key);
            cout << "Registro encontrado - Chave: " << key << ", Valor: " << record.value << endl;
        } catch (const exception& e) {
            cout << "Erro ao recuperar chave " << key << ": " << e.what() << endl;
        }
    }

    // Caso 2: Teste com valores não sequenciais
    cout << "\n[Teste 3] Criando novo arquivo com valores não sequenciais:" << endl;

    // Cria um novo gerenciador para um conjunto diferente
    BTreeFileManager<int, IntegerRecord> manager2("integers_random.dat", "integers_random_index.dat");

    // Insere valores não sequenciais para demonstrar balanceamento
    vector<int> nonSequentialValues = {50, 25, 75, 12, 37, 62, 87, 6, 18, 31, 43, 56, 68, 81, 93};

    for (int val : nonSequentialValues) {
        IntegerRecord record{val * 100};  // Valor armazenado é 100 vezes a chave
        manager2.insertRecord(record, val);
        cout << "Inserido: chave=" << val << ", valor=" << record.value << endl;
    }

    // Exibe a estrutura da árvore B para os valores não sequenciais
    cout << "\nEstrutura da árvore B após inserções não sequenciais:" << endl;
    manager2.printIndexStructure();

    // Verifica se os arquivos foram criados
    cout << "\n[Resultado Final]" << endl;
    cout << "Arquivos binários criados:" << endl;
    cout << "- integers.dat (dados sequenciais)" << endl;
    cout << "- integers_index.dat (índice para dados sequenciais)" << endl;
    cout << "- integers_random.dat (dados não sequenciais)" << endl;
    cout << "- integers_random_index.dat (índice para dados não sequenciais)" << endl;

    return 0;
}
//
//// Função para exibir o menu
//void displayMenu() {
//    cout << "\n=== SISTEMA DE GERENCIAMENTO DE LIVROS COM ÁRVORE B ===" << endl;
//    cout << "1. Criar banco de dados a partir de CSV" << endl;
//    cout << "2. Consultar livro por ID" << endl;
//    cout << "3. Inserir novo livro" << endl;
//    cout << "4. Exibir estrutura da árvore B" << endl;
//    cout << "5. Listar todos os livros (amostra)" << endl;
//    cout << "0. Sair" << endl;
//    cout << "Escolha uma opção: ";
//}
//
//// Função para limpar o buffer de entrada
//void clearInputBuffer() {
//    cin.clear();
//    cin.ignore(numeric_limits<streamsize>::max(), '\n');
//}
//
//int main() {
//    // Cria um gerenciador de arquivos usando BTree para livros
//    BTreeFileManager<int, Book> bookManager("btree_books.dat", "btree_index.dat");
//
//    int choice = -1;
//    string csvPath;
//    int bookId;
//
//    while (choice != 0) {
//        displayMenu();
//        cin >> choice;
//        clearInputBuffer();
//
//        switch (choice) {
//            case 1: {
//                cout << "\n=== Criar banco de dados a partir de CSV ===" << endl;
//                cout << "Digite o caminho do arquivo CSV: ";
//                getline(cin, csvPath);
//
//                try {
//                    // Define um parser para arquivos CSV de livros
//                    auto bookParser = [](const string& line) -> pair<Book, int> {
//                        Book book;
//                        stringstream ss(line);
//                        string token;
//
//                        // ID
//                        if (!getline(ss, token, ';')) throw runtime_error("Formato de CSV inválido");
//                        if (token.empty()) throw runtime_error("ID inválido");
//                        book.id = stoi(token);
//
//                        // Title
//                        if (!getline(ss, book.title, ';')) throw runtime_error("Formato de CSV inválido");
//
//                        // Authors
//                        if (!getline(ss, book.authors, ';')) throw runtime_error("Formato de CSV inválido");
//
//                        // Year
//                        if (!getline(ss, token, ';')) throw runtime_error("Formato de CSV inválido");
//                        if (token.empty() || token == "") {
//                            book.year = 0;
//                        } else {
//                            try {
//                                book.year = stoi(token);
//                            } catch (...) {
//                                book.year = 0;
//                            }
//                        }
//
//                        // Categories
//                        getline(ss, book.categories);
//
//                        return {book, book.id};
//                    };
//
//                    cout << "Importando de " << csvPath << "..." << endl;
//                    bookManager.createFromCSV(csvPath, bookParser);
//
//                    cout << "Importação concluída. Total de registros: " << bookManager.getIndexSize() << endl;
//                } catch (const exception& e) {
//                    cerr << "Erro: " << e.what() << endl;
//                }
//                break;
//            }
//
//            case 2: {
//                cout << "\n=== Consultar livro por ID ===" << endl;
//                cout << "Digite o ID do livro: ";
//                cin >> bookId;
//                clearInputBuffer();
//
//                try {
//                    Book book = bookManager.getRecordByKey(bookId);
//                    cout << "\nLivro encontrado:" << endl;
//                    cout << "ID: " << book.id << endl;
//                    cout << "Título: " << book.title << endl;
//                    cout << "Autor(es): " << book.authors << endl;
//                    cout << "Ano: " << book.year << endl;
//                    cout << "Categorias: " << book.categories << endl;
//                } catch (const exception& e) {
//                    cout << "Erro: " << e.what() << endl;
//                }
//                break;
//            }
//
//            case 3: {
//                cout << "\n=== Inserir novo livro ===" << endl;
//                Book newBook;
//
//                cout << "ID: ";
//                cin >> newBook.id;
//                clearInputBuffer();
//
//                cout << "Título: ";
//                getline(cin, newBook.title);
//
//                cout << "Autor(es): ";
//                getline(cin, newBook.authors);
//
//                cout << "Ano: ";
//                cin >> newBook.year;
//                clearInputBuffer();
//
//                cout << "Categorias: ";
//                getline(cin, newBook.categories);
//
//                try {
//                    bookManager.insertRecord(newBook, newBook.id);
//                    cout << "Livro inserido com sucesso!" << endl;
//                } catch (const exception& e) {
//                    cout << "Erro ao inserir livro: " << e.what() << endl;
//                }
//                break;
//            }
//
//            case 4: {
//                cout << "\n=== Estrutura da árvore B ===" << endl;
//                bookManager.printIndexStructure();
//                break;
//            }
//
//            case 5: {
//                cout << "\n=== Listar livros (amostra) ===" << endl;
//                cout << "Digite o número inicial de IDs a listar: ";
//                int count;
//                cin >> count;
//                clearInputBuffer();
//
//                int found = 0;
//                for (int i = 1; found < count && i < 100000; i++) {
//                    try {
//                        Book book = bookManager.getRecordByKey(i);
//                        cout << "ID: " << book.id << " - \"" << book.title << "\" (" << book.year << ")" << endl;
//                        found++;
//                    } catch (...) {
//                        // Ignora registros não encontrados
//                    }
//                }
//
//                if (found == 0) {
//                    cout << "Nenhum livro encontrado. Verifique se o banco de dados foi criado." << endl;
//                }
//                break;
//            }
//
//            case 0:
//                cout << "Encerrando o programa..." << endl;
//                break;
//
//            default:
//                cout << "Opção inválida! Tente novamente." << endl;
//        }
//    }
//
//    return 0;
//}