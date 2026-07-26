# LAB06 OSA — Árvore B

Implementação acadêmica, em C++11, de uma Árvore B parametrizada pelo grau mínimo `t`. A estrutura associa uma chave a um endereço de arquivo e inclui busca, inserção, remoção, split, merge e empréstimo entre irmãos.

## Estado da implementação

**Snapshot educacional com problemas conhecidos de correção e propriedade de memória. Não use como índice confiável sem corrigir e testar os pontos abaixo.**

A complexidade pretendida é logarítmica, mas ela só vale quando todas as invariantes da Árvore B são preservadas.

## Invariantes e complexidade pretendida

- cada nó, exceto a raiz, possui entre `t - 1` e `2t - 1` chaves;
- um nó interno com `k` chaves possui `k + 1` filhos;
- todas as folhas permanecem na mesma profundidade;
- busca, inserção e remoção: `O(log_t n)`;
- split/merge de um nó: `O(t)`;
- memória: `O(n)`.

## Auditoria de memória e correção

Foram identificados riscos concretos no código atual:

- `splitChild` reduz o vetor do filho e depois tenta ler a mediana removida, causando acesso fora dos limites;
- `Node` destrói filhos recursivamente, enquanto `BTree::~BTree` também percorre e destrói os mesmos nós: há risco de **double free**;
- `merge` transfere ponteiros de filhos e apaga o irmão sem esvaziar sua lista; o destrutor do irmão pode apagar filhos que acabaram de ser transferidos;
- ao reduzir a raiz, um filho é promovido e o nó antigo é apagado, mas seu destrutor ainda é dono do filho promovido, deixando a nova raiz pendente;
- rotinas de empréstimo escrevem em posições de `vector` sem garantir antes o novo tamanho;
- a classe não explicita semântica de cópia/movimento, o que é perigoso para uma estrutura proprietária de ponteiros.

Esses problemas podem aparecer como crash, corrupção silenciosa ou vazamento. O primeiro objetivo deve ser tornar a propriedade exclusiva e verificável, idealmente com `std::unique_ptr`.

A execução do exemplo com AddressSanitizer reproduz um `container-overflow` em `splitChild` durante as inserções, confirmando que não se trata apenas de risco teórico.

## Compilar

```bash
make
./bin/btree
make clean
```

Build de diagnóstico:

```bash
make clean
make CXXFLAGS="-Wall -Wextra -std=c++11 -Iinclude -fsanitize=address,undefined"
./bin/btree
```

## Critérios mínimos para considerar a estrutura correta

1. salvar a mediana antes de redimensionar vetores;
2. escolher um único responsável pela destruição dos filhos;
3. implementar transferências de propriedade seguras em split/merge/rotação;
4. bloquear cópia ou aplicar a Regra dos Cinco;
5. testar inserções e remoções aleatórias contra `std::map`;
6. validar invariantes após cada operação;
7. executar testes com AddressSanitizer e UndefinedBehaviorSanitizer.

## Contexto

Este laboratório integra o hub [OSA-projects](https://github.com/jotavsevla/OSA-projects). A estrutura representa a etapa de indexação balanceada do percurso conceitual `registro → offset → índice`.
