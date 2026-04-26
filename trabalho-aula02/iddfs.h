// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito

#ifndef IDDFS_H
#define IDDFS_H

#include "game.h"
#include "tree.h"
#include <stdbool.h>

// Estrutura de pilha utilizada internamente pelo IDDFS
// Cada item guarda um nó da árvore de busca e um ponteiro para o próximo item

typedef struct StackItem {
    node              nd;
    struct StackItem* next;
} StackItem;

typedef struct {
    StackItem* top;
    int        size;
} Stack;

Stack* newStack(void);
void   pushStack(Stack* s, node nd);
node   popStack(Stack* s);
void   freeStack(Stack* s);

// Estrutura para rastrear os estados presentes no caminho atual da busca
// Eh usada pelo DLS para evitar ciclos

typedef struct PathItem {
    game* state;
    struct PathItem* next;
} PathItem;

typedef struct {
    PathItem* top;
} PathSet;

PathSet* newPathSet(void);
void pushPath(PathSet* ps, game* state);
void popPath(PathSet* ps);
bool inPath(PathSet* ps, game* state);
void freePathSet(PathSet* ps);

// Contadores globais para medir o desempenho da busca
// g_nodes_expanded conta os nós da iteração atual
// g_total_nodes acumula todas as iterações
extern long g_nodes_expanded;
extern long g_total_nodes;

// DLS: busca em profundidade com limite fixo
node DLS(node nd, int depth, PathSet* ps);

// IDDFS: aprofundamento iterativo de 0 até maxDepth
node IDDFS(game* G, int maxDepth);

// Wrapper para uso no searchGame.c
node IDDFSSearch(game* G);

// Profundidade usada por IDDFSSearch. Valor 31 cobre todos os casos do 8-puzzle
#define MAX_DEPTH_IDDFS 31

#endif /* IDDFS_H */
