// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito
#include "tree.h"

#ifndef BFSTREE_H
#define BFSTREE_H

struct Item{
    node nd;
    struct Item* next;
};

typedef struct Item* item;

typedef struct{
    item first, last;
} Fila;

typedef Fila* fila;

fila newFila();
void insert(fila f, node newNode);
node pop(fila f);
// Implementação da função removerNo e modificação na função isIn explicada em bfstree.c
void removerNo(fila f, item alvo);
bool isIn(fila f, node nd);

node BFS(game* G);

#endif