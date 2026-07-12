// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito

// Esse arquivo implementa o agente que explora o grid em busca do ouro. Ele começa em (0,0), se move célula a célula pelas vizinhas, sente cheiro/vento
// como dica de perigos próximos (monstro/buraco) e ganha ou perde pontos (score).

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "agent.h"


// Flag global que liga/desliga as mensagens de eventos (matar monstro, cair no buraco, achar o ouro, etc.).
bool agentVerbose = true;

agent newAgent(enviroment E){
    agent A;
    A.score = 0;
    A.comOuro = false;
    A.temFlecha = true;
    A.onde = &E.grid[0][0];
    return A;
}

void sense(agent A){
    if (A.onde->S.cheiro){
        printf("Aqui há um cheiro monstruoso. \n");
    }
    if (A.onde->S.vento){
        printf("Aqui está batendo um vento estranho. \n");
    }
}

bool move(agent* A, enviroment E, place* target){
/*
    Movimenta o agente para a posição place, desde que esta seja uma vizinha de
    cleaner.ondeCleaner e que a bateria de C não esteja vazia.
*/
    if (isNeighbor(*A->onde,*target)){
        A->onde = target;
        A->score--;
        if (A->onde->monstro){
            if (A->temFlecha){
                if (agentVerbose) printf("Matou o monstro. \n");
                A->score -= 10;
                A->onde->monstro = false;
                A->temFlecha = false; // flecha única: usada, não sobra mais
            }
            else{
                if (agentVerbose) printf("Pego pelo monstro.\n");
                A->score -= 1000;
            }
        }
        if (A->onde->buraco){
            if (agentVerbose) printf("Caiu no buraco. \n");
            A->score -= 1000;
        }
        if (A->onde->ouro){
            if (agentVerbose) printf("O ouro está aqui! \n");
            A->comOuro = true;
            A->score += 1000;
            A->onde->ouro = false; // evita somar +1000 de novo se revisitar a célula
        }
        if (A->comOuro && A->onde->row==E.h-1 && A->onde->col==E.w-1){
            if (agentVerbose) printf("Escapou com o ouro! Parabéns. \n");
        }
        return true;
    }
    return false;
}

void printSimulation(agent A, enviroment E){
    for (int i=0; i<E.h; i++){
        for (int j=0; j<E.w; j++){
            if (&E.grid[i][j]==A.onde){
                printf("O ");
            }
            else{
                printf("_ ");
            }
        }
        printf("\n");
    }
}