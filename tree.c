#include "tree.h"
#include <stdlib.h>
#include <assert.h>

node childNode(node parent, int num){
    //Inicialização
    node child = malloc(sizeof(struct Node));
    assert(child);    
    child->state = newGame();
    child->parent = parent;
    //Se parent é NULL, cria o primeiro nó da árvore
    if (parent!=NULL){
        copyGame(parent->state,child->state);
        moveGame(child->state,num);
        child->action = num;
        child->pathCost = parent->pathCost+1;
    }
    else{
        child->action = -1;
        child->pathCost = 0;
    }
    return child;
}

void delNode(node n){
    if (n!=NULL){
        delGame(n->state);
        free(n);
    }
}


