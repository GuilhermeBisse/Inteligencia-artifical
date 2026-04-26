// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito

#ifndef IDDFS_H
#define IDDFS_H

#include "game.h"
#include "tree.h"
#include <stdbool.h>

/*
 * iddfs.h — Busca em Profundidade com Aprofundamento Iterativo (IDDFS)
 *
 * Para cada limite de profundidade d = 0, 1, 2, ..., maxDepth:
 *   Executa DLS com limite d.
 *   Se encontrar a solução → retorna o nó.
 *   Se não encontrar → aumenta d e repete.
 *
 */
 
/* Pilha simples */

typedef struct StackItem {
    node nd;
    struct StackItem* next;
} StackItem;

typedef struct {
    StackItem* top;
    int size;
} Stack;

Stack* newStack(void);
void   pushStack(Stack* s, node nd);
node   popStack(Stack* s);
void   freeStack(Stack* s);

/* Conjunto de estados no caminho atual (evita ciclos) */

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


/* Contadores globais para análise */

extern long g_nodes_expanded;
extern long g_total_nodes;

/* DLS recursivo com limite de profundidade */
node DLS(node nd, int depth, PathSet* ps);

/* IDDFS: itera d de 0 até o maxDepth */
node IDDFS(game* G, int maxDepth);

node IDDFSSearch(game* G);

/* Profundidade máxima usada por IDDFSSearch */
#define MAX_DEPTH_IDDFS 31

#endif /* IDDFS_H */
