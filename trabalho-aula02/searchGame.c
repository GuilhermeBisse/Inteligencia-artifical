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
// É aqui no searchGame.c que realizamos os experimentos para comparar os algoritmos A* e IDA* utilizando as heurísticas Manhattan e Fora do Lugar. Para isso, definimos um ponteiro para função heuristica, que é utilizado tanto no A* quanto no IDA* para calcular o valor da heurística em cada nó. No início de cada experimento, inicializamos o jogo com uma configuração aleatória, e depois executamos o A* e o IDA* utilizando cada uma das heurísticas, medindo o tempo gasto e o número de passos para encontrar a solução. Os resultados são armazenados em um arquivo CSV para posterior análise.

// Função para realizar os experimentos, comparando os algoritmos A* e IDA* utilizando as heurísticas Manhattan e Fora do Lugar. Os resultados são armazenados em um arquivo CSV para posterior análise.
void realizarExperimentos() {
    FILE *arq = fopen("resultados_puzzle.csv", "w");
    if (arq == NULL) {
        printf("Erro ao abrir o ficheiro!\n");
        return;
    }

    fprintf(arq, "Experimento;Algoritmo;Heuristica;Tempo(s);Passos\n");
    srand(time(NULL));

    game* G = newGame();
    game* copia = newGame(); // importante

    for (int i = 1; i <= 20; i++) {
        printf("\n--- Experimento %d ---\n", i);

        initGame(G);
        printGame(G);

        // MANHATTAN

        // A*
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

        // IDA*
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

        // FORA DO LUGAR

        // A*
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

        // IDA*
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
        
        // IDDFS
        copyGame(G, copia);
        start = clock();
        node solucao_IDDFS = IDDFSSearch(copia);
        end = clock();
        tempo = (double)(end - start) / CLOCKS_PER_SEC;
        if (solucao_IDDFS != NULL)
            fprintf(arq, "%d;IDDFS;Nenhuma;%.6f;%d\n", i, tempo, solucao_IDDFS->pathCost);
        else
            fprintf(arq, "%d;IDDFS;Nenhuma;%.6f;FALHOU\n", i, tempo);
        
        // DFS
        node root = childNode(NULL, 0);
        copyGame(G, root->state);

        start = clock();
        node solucao_DFS = DFS_Limited(root, 20);
        end = clock();

        tempo = (double)(end - start) / CLOCKS_PER_SEC;

        if (solucao_DFS != NULL)
            fprintf(arq, "%d;DFS;SemHeuristica;%.6f;%d\n", i, tempo, solucao_DFS->pathCost);
        else
            fprintf(arq, "%d;DFS;SemHeuristica;%.6f;FALHOU\n", i, tempo);
   
        // IDS
        copyGame(G, copia);

        start = clock();
        node solucao_IDS = IDS(copia);
        end = clock();

        tempo = (double)(end - start) / CLOCKS_PER_SEC;

        if (solucao_IDS != NULL)
            fprintf(arq, "%d;IDS;SemHeuristica;%.6f;%d\n", i, tempo, solucao_IDS->pathCost);
        else
            fprintf(arq, "%d;IDS;SemHeuristica;%.6f;FALHOU\n", i, tempo);

        // Busca Bidirecional (bidir)
        copyGame(G, copia);
        
        start = clock();
        node solucao_bidir = bidirectionalSearch(copia, objetivo);
        end = clock();
        
        tempo = (double)(end - start) / CLOCKS_PER_SEC;
        
        if (solucao_bidir != NULL)
            fprintf(arq, "%d;Bidirecional;BFS;%.6f;%d\n", i, tempo, solucao_bidir->pathCost);
        else
            fprintf(arq, "%d;Bidirecional;BFS;%.6f;FALHOU\n", i, tempo);
    }
    

    fclose(arq);
    delGame(G);
    delGame(copia);

    printf("\nProcesso concluido em 'resultados_puzzle.csv'\n");
}

// A função main do nosso programa.
int main() {
    realizarExperimentos();
    return 0;
}
