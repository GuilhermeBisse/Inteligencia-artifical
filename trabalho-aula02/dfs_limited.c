// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito
#include "game.h"
#include "tree.h"
#include <stdbool.h>
#include <stdlib.h>

node DFS_Limited(node parent, int limit){

    // Verifica se o estado atual é o objetivo
    if (endGame(parent->state)){
        return parent;
    }

    if (limit == 0){
        return NULL;
    }

    // Cria uma cópia do estado atual para testar movimentos
    game* aux = newGame();
    copyGame(parent->state, aux);

    for (int k = 1; k < 9; k++){

        if (moveGame(aux, k)){

            // Cria um novo nó filho com esse movimento
            node child = childNode(parent, k); 

            // Evita voltar para o estado do pai (loop imediato)
            if (parent->parent != NULL &&
                equal(child->state, parent->parent->state)) {
                delNode(child);
                moveGame(aux, k);
                continue;
            }

            // Chamada recursiva diminuindo o limite
            node result = DFS_Limited(child, limit - 1); 

            if (result != NULL){
                delGame(aux);
                return result;
            }

            delNode(child);

            moveGame(aux, k); 
        }
    }

    delGame(aux);
    return NULL;
}