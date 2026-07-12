// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "agent.h"
#include "env.h"
#include "knowledge.h"

/*
    Roda uma simulação completa com o agente autônomo baseado em inferência.
    Retorna o score final (utilidade) e informa via ponteiros o número de
    movimentos usados e se o agente conseguiu escapar com o ouro.
*/
int runEpisode(enviroment E, int maxMoves, int* movesUsed, bool* solved){
    agent A = newAgent(E);
    KB kb = newKB(E.h, E.w);
    updateKB(&kb, A.onde); // observa a posição inicial (0,0)

    int moves = 0;
    bool done = (A.comOuro && A.onde->row==E.h-1 && A.onde->col==E.w-1);

    while (!done && moves < maxMoves){
        place* target = chooseTarget(kb, E, A, A.comOuro);
        if (target == NULL) break; // sem alternativa (não deveria ocorrer em grid retangular)

        move(&A, E, target); // já imprime "Matou o monstro", "Caiu no buraco", etc. (agentVerbose=true)
        updateKB(&kb, A.onde);
        moves++;

        done = (A.comOuro && A.onde->row==E.h-1 && A.onde->col==E.w-1);
    }

    *movesUsed = moves;
    *solved = done;
    int finalScore = A.score;
    delKB(&kb);
    return finalScore;
}

int main(int argc, char** argv){
    int sizes[5] = {4, 5, 6, 7, 8};   // 5 tamanhos distintos de sala (quadrada h=w)
    int numRuns = 10;                 // 10 configurações iniciais por tamanho
    unsigned int seed = 2024;         // seed fixa -> resultados reprodutíveis

    // resultados por tamanho, calculados ao final de cada bloco
    double meanScoreArr[5], stdScoreArr[5], meanMovesArr[5], stdMovesArr[5], successRateArr[5];

    srand(seed);
    agentVerbose = true; // imprime os eventos de cada movimento (Matou o monstro, Caiu no buraco, etc.)

    FILE* raw = fopen("results_raw.csv", "w");
    FILE* summary = fopen("results_summary.csv", "w");
    fprintf(raw, "size,run,score,moves,solved\n");
    fprintf(summary, "size,mean_score,std_score,mean_moves,std_moves,success_rate\n");

    for (int s=0; s<5; s++){
        int h = sizes[s], w = sizes[s];
        int area = h*w;
        int maxMoves = 10*area;           // limite = 10x o tamanho (área) da sala
        int numMonstro = h/3; if (numMonstro < 1) numMonstro = 1;
        int numBuraco = area/8; if (numBuraco < 1) numBuraco = 1;

        printf("\n############################################################\n");
        printf("# TAMANHO %dx%d  (limite de movimentos = %d)\n", h, w, maxMoves);
        printf("############################################################\n");

        double sumScore=0, sumScore2=0, sumMoves=0, sumMoves2=0;
        int successes = 0;

        for (int run=0; run<numRuns; run++){
            enviroment E = newEnviroment(h, w);
            initEnviromentNoSeed(E, numBuraco, numMonstro);

            printf("\n=== Sala %dx%d | configuracao %d/%d (buracos=%d, monstros=%d) ===\n",
                h, w, run+1, numRuns, numBuraco, numMonstro);

            int moves; bool solved;
            int score = runEpisode(E, maxMoves, &moves, &solved);

            printf(">>> Resultado: score=%d | movimentos=%d/%d | %s\n",
                score, moves, maxMoves, solved ? "SUCESSO (escapou com o ouro)" : "FALHOU (limite de movimentos)");

            sumScore += score; sumScore2 += (double)score*score;
            sumMoves += moves; sumMoves2 += (double)moves*moves;
            if (solved) successes++;

            fprintf(raw, "%d,%d,%d,%d,%d\n", sizes[s], run, score, moves, solved?1:0);

            delEnviroment(&E);
        }

        double meanScore = sumScore/numRuns;
        double varScore = sumScore2/numRuns - meanScore*meanScore;
        double stdScore = varScore>0 ? sqrt(varScore) : 0.0;

        double meanMoves = sumMoves/numRuns;
        double varMoves = sumMoves2/numRuns - meanMoves*meanMoves;
        double stdMoves = varMoves>0 ? sqrt(varMoves) : 0.0;

        double successRate = 100.0*successes/numRuns;

        meanScoreArr[s]=meanScore; stdScoreArr[s]=stdScore;
        meanMovesArr[s]=meanMoves; stdMovesArr[s]=stdMoves;
        successRateArr[s]=successRate;

        fprintf(summary, "%d,%.4f,%.4f,%.4f,%.4f,%.2f\n",
            sizes[s], meanScore, stdScore, meanMoves, stdMoves, successRate);
    }

    fclose(raw);
    fclose(summary);

    printf("\n\n============================================================\n");
    printf("RESUMO FINAL (media e desvio padrao de cada tamanho)\n");
    printf("============================================================\n");
    printf("%-6s %-10s %-10s %-10s %-10s %-8s\n",
        "size", "meanScore", "stdScore", "meanMoves", "stdMoves", "succ%%");
    for (int s=0; s<5; s++){
        printf("%-6d %-10.2f %-10.2f %-10.2f %-10.2f %-8.1f\n",
            sizes[s], meanScoreArr[s], stdScoreArr[s], meanMovesArr[s], stdMovesArr[s], successRateArr[s]);
    }

    printf("\nArquivos gerados: results_raw.csv, results_summary.csv\n");
    return 0;
}