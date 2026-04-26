// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <windows.h> função sleep no windows
#include "game.h"
#include "tree.h"
#include "bfstree.h"
#include "greedy.h"

// Mudança nessa função, que inicialmente era WinDist, representa o h_fora mencionado nos slides (uma das heurísticas a serem analisadas). Ela retorna o número de peças fora do lugar em relação ao estado objetivo, e é utilizada como heurística para o algoritmo de busca gulosa. A implementação é simples, basta percorrer o grid do jogo e contar quantas peças estão fora do lugar, ou seja, quantas peças não estão na posição que deveriam estar no estado objetivo.
int hfora(game* G){
/*
    Retorna número de peças fora do lugar em relação ao estado objetivo.
*/
    int dist = 0;
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++){
            if (G->grid[i][j] != 0 && G->grid[i][j] != i*3 + j){
                dist++;
            }
        }
    return dist;
}

// Implementação da função hm, que representa a heurística Manhattan mencionada nos slides. Ela retorna a soma das distâncias de Manhattan de cada peça em relação à sua posição correta no estado objetivo. A distância de Manhattan é calculada como a soma das diferenças absolutas entre as coordenadas da peça e as coordenadas da posição correta. Para cada peça, calculamos sua posição correta com base no seu valor (por exemplo, a peça 1 deve estar na posição (0,1), a peça 2 deve estar na posição (0,2), etc.), e somamos as distâncias de Manhattan para todas as peças para obter o valor da heurística.
int hm(game* G){
    int dist = 0;
    for (int i=0; i<3; i++){
        for(int j=0;j<3;j++){
            int elem = G->grid[i][j];
            if (elem!=0){
                int targetI = elem/3;
                int targetJ = elem%3;
                dist += abs(i - targetI) + abs(j - targetJ);
            }
        }
    }
    return dist;
}

node greedySearch(game* G){
    //Inicializa primeiro nó com a configuração fornecida do jogo.
    node nd = childNode(NULL,0);
    copyGame(G,nd->state);
    if (endGame(G)){
        return nd;
    }
    //Armazena somente um nó, já que cada um possui um ponteiro para o anterior
    node parent = nd;

    while(parent!=NULL){
        //Variável auxiliar 
        game* aux = newGame();
        copyGame(parent->state,aux);
        //Ponteiro para armazenar o filho mais próximo do objetivo
        node optChild = NULL;
        //Para armazenar menor distância
        int dist = 1000;
        //Cria nós filhos para cada ação possível
        for (int k=1; k<9; k++){
            //Checa se movimento é válido
            if (moveGame(aux,k)){
                //Cria filho
                node auxChild = childNode(parent,k);
                //Se é a menor distância até aqui, armazena
                if (hfora(auxChild->state)<dist){
                    dist = hfora(auxChild->state);
                    if (optChild!=NULL){
                        delNode(optChild);
                    }
                    optChild = auxChild;
                }
                //Caso contrário, libera memória
                else{
                    delNode(auxChild);
                }
                moveGame(aux,k);
            }
        }
        //Verifica se resolveu o problema
        if (endGame(optChild->state))
            return optChild;
        //Nó optimal será o expandido
        parent = optChild;
        //sleep(1);
        //printGame(parent->state); printf("\n");
        //Libera memória da variável auxiliar
        printf("%d \n",parent->pathCost);
        delGame(aux);
    }
    return NULL;
}