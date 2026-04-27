// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

game* newGame(){
/*
    Inicialização de um jogo.
*/
    game* G = malloc(sizeof(game));
    assert(G);
    G->grid = malloc(3*sizeof(int*));
    assert(G->grid);
    G->grid[0] = malloc(3*sizeof(int));
    G->grid[1] = malloc(3*sizeof(int));
    G->grid[2] = malloc(3*sizeof(int));
    return G;
}

/* Implementamos também a função de verificar a paridade do jogo mencionada no vídeo que o prof. Emílio passou no classroom, que é utilizada para garantir que o jogo gerado seja solucionável. O número de inversões é contado, e o jogo é considerado solucionável se o número de inversões for par.
*/

bool verificaParidade(game* G){
    int inversoes = 0;
    int v[9];
    int idx = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            v[idx++] = G->grid[i][j];
        }
    }

    for (int i = 0; i < 9; i++) {
        for (int j = i + 1; j < 9; j++) {
            if (v[i] != 0 && v[j] != 0 && v[i] > v[j]) {
                inversoes++;
            }
        }
    }
    return inversoes%2==0;
}

void delGame(game* G){
    free(G->grid[0]);
    free(G->grid[1]);
    free(G->grid[2]);
    free(G->grid);
    free(G);
}

void initGame(game* G){
    bool soluvel = false;
    while(!soluvel){
        for (int i=0; i<3; i++)
            for (int j=0; j<3; j++)
                G->grid[i][j] = 0;

        //Inclui números de 1 a 8 aleatoriamente
        for (int k=1; k<9; k++){
            int i = rand()%3;
            int j = rand()%3;
            while (G->grid[i][j]!=k){
                if (G->grid[i][j]==0)
                    G->grid[i][j] = k;
                else{
                    i = rand()%3;
                    j = rand()%3;
                }
            }
        }
        soluvel = verificaParidade(G);
    }
}

bool endGame(game* G){
/*
    Checa se o jogo foi vencido.
*/
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++){
            if (G->grid[i][j]!=i*3+j)
                return false;
        }
    printf("Você venceu o jogo! \n");
    return true;
}

void printGame(game* G){
/*
    Imprime estado atual do jogo na tela
*/
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            printf("%d  ",G->grid[i][j]);
        }
        printf("\n");
    }
}

void copyGame(game* G, game* target){
    //Copia G para target
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++){
            target->grid[i][j] = G->grid[i][j];
        }
}

bool moveGame(game* G, int num) {
    // zie e zjc são coordenadas do espaço vazio
    // ni e nj são coordenadas do número que queremos mover
    int zi, zj, ni, nj;

    // Procura as posições do vazio (0) e da peça escolhida (num) na grade 3x3
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // Encontrou o espaço vazio? Save na linha e coluna
            if (G->grid[i][j] == 0) { 
                zi = i; 
                zj = j; 
            }
            // Encontrou a peça com o valor 'num'? Save na linha e coluna
            if (G->grid[i][j] == num) { 
                ni = i; 
                nj = j; 
            }
        }
    }

    // Verifica quantos "passos" a peça está do buraco (apenas horizontal e vertical)
    int dist = abs(zi - ni) + abs(zj - nj);

    // Se a distância for 1, significa que a peça está imediatamente ao lado, acima ou abaixo do vazio
    if (dist == 1) {
        // Swap
        G->grid[zi][zj] = num; 
        G->grid[ni][nj] = 0;   
        return true;           
    }
    
    // Se a distância não for 1 (peça longe ou na diagonal), o movimento é inválido
    return false;
}

bool equal(game* A, game* B){
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++){
            if (A->grid[i][j]!=B->grid[i][j]){
                return false;
            }
        }
    return true;
}
