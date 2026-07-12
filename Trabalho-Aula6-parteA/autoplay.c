// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito


#include <stdio.h>
#include <stdlib.h>
#include "agent.h"
#include "env.h"
#include "knowledge.h"

// Roda a simulação do agente autônomo: ele cria o ambiente e o agente, e a cada passo usa a base de conhecimento (KB) para escolher 
// um alvo (chooseTarget) e se mover até achar o ouro e escapar, ou até estourar o limite de movimentos.

int main(){
    srand(7); // seed fixa só para o demo ser reprodutível

    int h=6, w=6, numBuraco=4, numMonstro=h/3;
    enviroment E = newEnviroment(h,w);
    initEnviromentNoSeed(E, numBuraco, numMonstro);

    agent A = newAgent(E);
    KB kb = newKB(E.h, E.w);
    updateKB(&kb, A.onde);

    printf("=== Agente autônomo (baseado em inferência) ===\n");
    printf("Sala %dx%d, %d buracos, %d monstro(s)\n\n", h, w, numBuraco, numMonstro);

    int maxMoves = 10*h*w, moves=0;
    bool done = false;
    while (!done && moves<maxMoves){
        printSimulation(A, E);
        sense(A);
        printf("Score atual: %d\n", A.score);

        place* target = chooseTarget(kb, E, A, A.comOuro);
        if (!target){ printf("Sem alvo possível, parando.\n"); break; }

        printf("--> movendo para (%d,%d)\n\n", target->row, target->col);
        move(&A, E, target);
        updateKB(&kb, A.onde);
        moves++;
        done = (A.comOuro && A.onde->row==E.h-1 && A.onde->col==E.w-1);
    }

    printSimulation(A, E);
    printf("\nSimulação encerrada em %d movimentos. Score final: %d. Sucesso: %s\n",
        moves, A.score, done?"SIM":"NAO");

    delKB(&kb);
    delEnviroment(&E);
    return 0;
}