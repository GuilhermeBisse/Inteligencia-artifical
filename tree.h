
#include "game.h"

#ifndef TREE_H
#define TREE_H

struct Node {
    game* state; //Configuração atual do jogo
    int action; //Ação que levou à configuração atual (argumento num de moveGame)
    struct Node* parent; //nó pai
    int pathCost; //Custo
};

typedef struct Node* node;

//Parto de um nó filho
node childNode(node parent, int num);
void delNode(node n);
#endif