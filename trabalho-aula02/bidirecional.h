#ifndef BIDIRECTIONAL_H
#define BIDIRECTIONAL_H

#include "game.h"
#include "tree.h"
#include "bfstree.h"

// Executa a busca a partir do início e do objetivo.
// Retorna o nó onde houve encontro (lado do início).
node bidirectionalSearch(game* start, game* goal);

// Imprime o caminho completo a partir dos dois lados.
void printBidirectionalPath(node meetIn, node meetOut);

#endif
