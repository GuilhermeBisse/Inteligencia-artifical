// Trabalho desenvolvido por [Seu Nome] - Implementação de Busca Bidirecional
#ifndef BIDIRECTIONAL_H
#define BIDIRECTIONAL_H

#include "game.h"
#include "tree.h"
#include "bfstree.h"

/* * Executa a busca bidirecional entre o estado inicial G e o objetivo.
 * Retorna o nó de encontro que liga as duas pontas da busca.
 */
node bidirectionalSearch(game* G, game* objetivo);

/*
 * Função auxiliar para reconstruir e imprimir o caminho completo 
 * unindo as duas árvores (a que partiu do início e a que partiu do fim).
 */
void printBidirectionalPath(node meetIn, node meetOut);

#endif