// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito

#include "game.h"
#include "tree.h"
#include "bfstree.h"
#include "greedy.h"
#include "dfs.h"
#include "ids.h"
#include "dfs_limited.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "astar.h"
#include <time.h>
#include <math.h>
#include "iddfs.h"
#include "bidirecional.h"

// Executa vários testes com tabuleiros aleatórios e salva os resultados em CSV
void realizarExperimentos() {
    FILE *arq = fopen("resultados_puzzle.csv", "w");
    if (arq == NULL) {
        printf("Erro ao abrir o ficheiro!\n");
        return;
    }

    fprintf(arq, "Experimento;Algoritmo;Heuristica;Tempo(s);Passos\n");
    srand(time(NULL));

    game* G = newGame();
    game* copia = newGame();
    game* objetivo = newGame(); // estado final fixo

    // monta manualmente o estado objetivo (mesmo padrão usado em endGame)
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            objetivo->grid[i][j] = i * 3 + j;

    for (int i = 1; i <= 20; i++) {
        printf("\n--- Experimento %d ---\n", i);

        initGame(G);   // gera um tabuleiro válido aleatório
        printGame(G);

        // A* (Manhattan)
        copyGame(G, copia);
        heuristica = hm;

        clock_t start = clock();
        node solucao_A = AStarSearch(copia);
        clock_t end = clock();

        double tempo = (double)(end - start) / CLOCKS_PER_SEC;

        if (solucao_A != NULL)
            fprintf(arq, "%d;A*;Manhattan;%.6f;%d\n", i, tempo, solucao_A->pathCost);
        else
            fprintf(arq, "%d;A*;Manhattan;%.6f;FALHOU\n", i, tempo);

        //  IDA* (Manhattan) 
        copyGame(G, copia);
        heuristica = hm;

        start = clock();
        node solucao_IDA = IDAStarSearch(copia);
        end = clock();

        tempo = (double)(end - start) / CLOCKS_PER_SEC;

        if (solucao_IDA != NULL)
            fprintf(arq, "%d;IDA*;Manhattan;%.6f;%d\n", i, tempo, solucao_IDA->pathCost);
        else
            fprintf(arq, "%d;IDA*;Manhattan;%.6f;FALHOU\n", i, tempo);

        //  A* (Fora do Lugar) 
        copyGame(G, copia);
        heuristica = hfora;

        start = clock();
        node solucao_A2 = AStarSearch(copia);
        end = clock();

        tempo = (double)(end - start) / CLOCKS_PER_SEC;

        if (solucao_A2 != NULL)
            fprintf(arq, "%d;A*;ForaLugar;%.6f;%d\n", i, tempo, solucao_A2->pathCost);
        else
            fprintf(arq, "%d;A*;ForaLugar;%.6f;FALHOU\n", i, tempo);

        //  IDA* (Fora do Lugar) 
        copyGame(G, copia);
        heuristica = hfora;

        start = clock();
        node solucao_IDA2 = IDAStarSearch(copia);
        end = clock();

        tempo = (double)(end - start) / CLOCKS_PER_SEC;

        if (solucao_IDA2 != NULL)
            fprintf(arq, "%d;IDA*;ForaLugar;%.6f;%d\n", i, tempo, solucao_IDA2->pathCost);
        else
            fprintf(arq, "%d;IDA*;ForaLugar;%.6f;FALHOU\n", i, tempo);

        //  IDDFS 
        copyGame(G, copia);

        start = clock();
        node solucao_IDDFS = IDDFSSearch(copia);
        end = clock();

        tempo = (double)(end - start) / CLOCKS_PER_SEC;

        if (solucao_IDDFS != NULL)
            fprintf(arq, "%d;IDDFS;Nenhuma;%.6f;%d\n", i, tempo, solucao_IDDFS->pathCost);
        else
            fprintf(arq, "%d;IDDFS;Nenhuma;%.6f;FALHOU\n", i, tempo);

        //  DFS limitado 
        node root = childNode(NULL, 0);
        copyGame(G, root->state);

        start = clock();
        node solucao_DFS = DFS_Limited(root, 31);
        end = clock();

        tempo = (double)(end - start) / CLOCKS_PER_SEC;

        if (solucao_DFS != NULL)
            fprintf(arq, "%d;DFS;SemHeuristica;%.6f;%d\n", i, tempo, solucao_DFS->pathCost);
        else
            fprintf(arq, "%d;DFS;SemHeuristica;%.6f;FALHOU\n", i, tempo);

        delNode(root);

        //  IDS 
        copyGame(G, copia);

        start = clock();
        node solucao_IDS = IDS(copia);
        end = clock();

        tempo = (double)(end - start) / CLOCKS_PER_SEC;

        if (solucao_IDS != NULL)
            fprintf(arq, "%d;IDS;SemHeuristica;%.6f;%d\n", i, tempo, solucao_IDS->pathCost);
        else
            fprintf(arq, "%d;IDS;SemHeuristica;%.6f;FALHOU\n", i, tempo);

        //  Busca Bidirecional 
        copyGame(G, copia);

        start = clock();
        node solucao_bidir = bidirectionalSearch(copia, objetivo);
        end = clock();

        tempo = (double)(end - start) / CLOCKS_PER_SEC;

        // soma o custo dos dois lados até o ponto de encontro
        if (solucao_bidir != NULL)
            fprintf(arq, "%d;Bidirecional;BFS;%.6f;%d\n", i, tempo, solucao_bidir->pathCost);
        else
            fprintf(arq, "%d;Bidirecional;BFS;%.6f;FALHOU\n", i, tempo);

        //  BFS 
        copyGame(G, copia);

        start = clock();
        node solucao_BFS = BFS(copia);
        end = clock();

        tempo = (double)(end - start) / CLOCKS_PER_SEC;

        if (solucao_BFS != NULL)
            fprintf(arq, "%d;BFS;SemHeuristica;%.6f;%d\n", i, tempo, solucao_BFS->pathCost);
        else
            fprintf(arq, "%d;BFS;SemHeuristica;%.6f;FALHOU\n", i, tempo);
    }

    fclose(arq);

    delGame(G);
    delGame(copia);
    delGame(objetivo);

    printf("\nProcesso concluido em 'resultados_puzzle.csv'\n");
}

int main() {
    realizarExperimentos();
    return 0;
}