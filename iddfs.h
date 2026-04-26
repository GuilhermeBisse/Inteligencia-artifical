#ifndef IDDFS_H
#define IDDFS_H

#include "game.h"
#include "tree.h"

/*
 * iddfs.h — Busca em Profundidade com Aprofundamento Iterativo
 *
 * Para cada limite de profundidade d = 0, 1, 2, ..., maxDepth:
 *   Executa DLS (Depth-Limited Search) com limite d.
 *   Se encontrar a solução → retorna o nó.
 *   Caso contrário → aumenta d e repete.
 *
 * Propriedades:
 *   Completude  : Sim (se solução existir e maxDepth for suficiente)
 *   Optimalidade: Sim para custos unitários (retorna menor nº de passos)
 *   Complexidade: Temporal O(b^d), Espacial O(d)
 */

/* Estrutura de pilha para DLS iterativo */
typedef struct StackItem {
    node            nd;
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

/* Histórico de estados visitados no caminho atual */
typedef struct PathItem {
    game*           state;
    struct PathItem* next;
} PathItem;

typedef struct {
    PathItem* top;
} PathSet;

PathSet* newPathSet(void);
void     pushPath(PathSet* ps, game* state);
void     popPath(PathSet* ps);
bool     inPath(PathSet* ps, game* state);
void     freePathSet(PathSet* ps);

/* Contadores globais para análise */
extern long g_nodes_expanded;
extern long g_total_nodes;

/* DLS recursivo com limite de profundidade */
node DLS(node nd, int depth, PathSet* ps);

/* IDDFS: itera d de 0 até maxDepth */
node IDDFS(game* G, int maxDepth);

#endif /* IDDFS_H */
