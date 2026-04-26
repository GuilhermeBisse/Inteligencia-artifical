// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <windows.h> função sleep no windows
#include "game.h"
#include "tree.h"
#include "bfstree.h"
#include "greedy.h"

#ifndef ASTAR_H
#define ASTAR_H

// Adicionamos um ponteiro para função heuristica, para que seja possível escolher a heurística a ser utilizada em tempo de execução. Ele é definido em searchGame.c, onde são realizados os experimentos.
extern int (*heuristica)(game*);

// Além da AStarSearch (Busca A*), também implementamos o IDAStarSearch (Busca A* com aprofundamento iterativo, ou IDA*) e uma função auxiliar para o IDA* (explicações em astar.c).
int func_recursiva_auxiliar(node n, int g, int limite, node* solucao, int pecaAnterior);
node AStarSearch(game* G);
node IDAStarSearch(game* G);

#endif