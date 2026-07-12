// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito

#include "env.h"
#include <time.h>
#include <stdio.h>
#include <unistd.h>

void setSensation(place* p, place* v){

    if (isNeighbor(*v,*p)){
        if (v->buraco)
            p->S.vento = true;
        if (v->monstro)
            p->S.cheiro = true;
    }
}

//Funções básicas do ambiente

enviroment newEnviroment(int h, int w){
    enviroment E;
    E.h = h; E.w = w;
    E.grid = malloc(E.h*sizeof(place*));
    if (E.grid!=NULL){
        for (int i=0; i<h; i++){
            E.grid[i] = malloc(E.w*sizeof(place));
            if (E.grid[i]!=NULL)
                for (int j=0; j<w; j++){
                    E.grid[i][j].buraco = false;
                    E.grid[i][j].monstro = false;
                    E.grid[i][j].ouro = false;
                    E.grid[i][j].S.cheiro = false;
                    E.grid[i][j].S.vento = false;
                    E.grid[i][j].row = i;
                    E.grid[i][j].col = j;
                }
        }
    }
    return E;
}

// Libera a memoria alocada para o grid do ambiente.
void delEnviroment(enviroment* E){
    if (E!=NULL){
        for (int i=0; i<E->h; i++){
            free(E->grid[i]);
        }
        free(E->grid);
    }
}

void initEnviroment(enviroment E, int numBuraco, int numMonstro){
/*
    Inicializa sujeiras no ambiente (seedando o gerador de números aleatórios).
    Usado no modo interativo (main.c), onde a gente uma vez.
*/
    srand(time(NULL));
    initEnviromentNoSeed(E, numBuraco, numMonstro);
}

static int collectFreeInteriorCells(enviroment E, int (*cand)[2]){
/*
    Reúne, num vetor de candidatos, todas as células internas (não bordas)
    que ainda estão livres. Retorna quantos candidatos foram encontrados.
*/
    int numCand = 0;
    for (int i=0; i<E.h; i++){
        for (int j=0; j<E.w; j++){
            if (i!=0 && i!=E.h-1 && j!=0 && j!=E.w-1){
                if (!E.grid[i][j].buraco && !E.grid[i][j].monstro && !E.grid[i][j].ouro){
                    cand[numCand][0] = i;
                    cand[numCand][1] = j;
                    numCand++;
                }
            }
        }
    }
    return numCand;
}

static void shuffleCoords(int (*cand)[2], int n){
    for (int i=n-1; i>0; i--){
        int j = rand()%(i+1);
        int tr = cand[i][0], tc = cand[i][1];
        cand[i][0] = cand[j][0]; cand[i][1] = cand[j][1];
        cand[j][0] = tr; cand[j][1] = tc;
    }
}

void initEnviromentNoSeed(enviroment E, int numBuraco, int numMonstro){
/*
    Igual ao initEnviroment, mas sem re-semear o gerador de números aleatórios.
    Precisamos para gerar várias configurações em sequência no caso do experimentos,
    já que chamar srand(time(NULL)) repetidamente no mesmo segundo geraria sempre a mesma configuração.

    Em vez de sortear por rejeição (loop que só para quando acerta uma célula livre — e que
    pode travar em loop infinito se pedirem mais buracos/monstros do que células disponíveis), reunimos as células
    internas livres num vetor, embaralhamos e vamos consumindo do início: primeiro os buracos, depois os monstros, 
    e no fim o ouro. Sempre reservamos pelo menos 1 célula livre para o ouro, mesmo que isso
    signifique gerar menos buracos/monstros.
*/
    int maxCand = E.h*E.w;
    int (*cand)[2] = malloc(maxCand*sizeof(*cand));

    int numCand = collectFreeInteriorCells(E, cand);
    shuffleCoords(cand, numCand);

    // Reserva 1 célula pro ouro: buraco+monstro não podem consumir tudo
    int budget = numCand - 1;
    if (budget < 0) budget = 0;

    if (numBuraco > budget) numBuraco = budget;
    int idx = 0;
    for (int k=0; k<numBuraco; k++){
        E.grid[cand[idx][0]][cand[idx][1]].buraco = true;
        idx++;
    }
    budget -= numBuraco;

    if (numMonstro > budget) numMonstro = budget;
    for (int k=0; k<numMonstro; k++){
        E.grid[cand[idx][0]][cand[idx][1]].monstro = true;
        idx++;
    }

    // Ouro: sorteia 1 célula entre as que sobraram livres (recalcula, pois buraco/monstro podem ter sido limitados acima)
    int numCand2 = collectFreeInteriorCells(E, cand);
    if (numCand2 > 0){
        int pick = rand()%numCand2;
        E.grid[cand[pick][0]][cand[pick][1]].ouro = true;
    }

    free(cand);
    initSensations(E);
}

void initSensations(enviroment E){
    //Itera sobre posições no grid
    for (int i=0; i<E.h; i++)
        for (int j=0; j<E.w; j++){
            //Itera sobre vizinhos
            for (int k=0; k<E.h; k++)
                for (int l=0; l<E.w; l++){
                    setSensation(&E.grid[i][j],&E.grid[k][l]);
                }
        }
}

bool isNeighbor(place p, place q){
/*
    Considerando vizinhança-4 (em cruz).
*/
    if (p.row==q.row || p.col==q.col){
        if (p.row==q.row+1 || p.row==q.row-1)
            return true;
        if (p.col==q.col+1 || p.col==q.col-1)
            return true;
    }
    return false;
}

place* getPlace(enviroment E, int i, int j){
/*
    Retorna ponteiro para place com indices i e j no grid.
*/
    return &E.grid[i][j];
}