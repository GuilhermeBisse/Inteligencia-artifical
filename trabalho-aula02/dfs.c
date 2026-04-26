// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito
#include "game.h"
#include "tree.h"
#include <stdlib.h>

node DFS(node parent){

    // Verifica se o estado atual já é o objetivo
    if (endGame(parent->state)){
        return parent;
    }

    game* aux = newGame();
    copyGame(parent->state, aux);

    for (int k = 1; k < 9; k++){

        if (moveGame(aux, k)){

            // Cria um novo nó filho com esse movimento
            node child = childNode(parent, k);

            // Chamada recursiva: continua a busca a partir do filho
            node result = DFS(child);

            if (result != NULL){
                return result;
            }

            delNode(child);

            // Desfaz o movimento
            moveGame(aux, k);
        }
    }

    delGame(aux);
    return NULL;
}