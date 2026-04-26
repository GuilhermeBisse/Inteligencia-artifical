// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito

#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(){

    bool win = false;
    game* G = newGame();
    initGame(G);
    int move;

    while (!win){
        printGame(G);
        printf("Qual peça você deseja mover? ");
        scanf("%d",&move);
        if (moveGame(G,move)){
            win = endGame(G);
        }
        else{
            printf("Movimento inválido. \n");
        }
    }

    return 0;
}