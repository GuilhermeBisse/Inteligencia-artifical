// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito

// Aqui testamos o IDDFS de forma isolada, com um único estado inicial aleatório e solucionável.
// O programa gera o tabuleiro, executa a busca e imprime quantos passos foram necessários,
// em que profundidade a solução foi encontrada, quantos nós foram expandidos e o tempo gasto.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "game.h"
#include "tree.h"
#include "iddfs.h"

#define MAX_DEPTH 31

// Timer que funciona tanto no Windows quanto no Linux/Mac.
#ifdef _WIN32
#  include <windows.h>
static double get_time_s(void) {
    LARGE_INTEGER freq, cnt;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&cnt);
    return (double)cnt.QuadPart / (double)freq.QuadPart;
}
#else
static double get_time_s(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}
#endif

// Conta o número de inversões no vetor flat, excluindo o zero.
// Uma configuração do 8-puzzle é solucionável se e somente se o número de inversões for par.
static int count_inversions(int flat[9]) {
    int inv = 0;
    for (int i = 0; i < 9; i++) {
        if (flat[i] == 0) continue;
        for (int j = i + 1; j < 9; j++) {
            if (flat[j] == 0) continue;
            if (flat[i] > flat[j]) inv++;
        }
    }
    return inv;
}

// Gera uma configuração inicial aleatória, solucionável e diferente do estado objetivo.
// Usamos o algoritmo Fisher-Yates para embaralhar e checamos a paridade das inversões,
// da mesma forma que a função verificaParidade em game.c.
static void generate_solvable(game* G) {
    int flat[9];
    do {
        for (int i = 0; i < 9; i++) flat[i] = i;
        for (int i = 8; i > 0; i--) {
            int j = rand() % (i + 1);
            int tmp = flat[i]; flat[i] = flat[j]; flat[j] = tmp;
        }
    } while (count_inversions(flat) % 2 != 0
          || (flat[0]==0 && flat[1]==1 && flat[2]==2 &&
              flat[3]==3 && flat[4]==4 && flat[5]==5 &&
              flat[6]==6 && flat[7]==7 && flat[8]==8));

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            G->grid[i][j] = flat[i * 3 + j];
}

// Percorre a cadeia de nós pai->filho para contar quantos passos a solução tem.
static int solution_length(node nd) {
    int steps = 0;
    for (node aux = nd; aux->parent != NULL; aux = aux->parent)
        steps++;
    return steps;
}

int main(void) {
    srand((unsigned int)time(NULL));

    game* G = newGame();
    generate_solvable(G);

    printf("Estado inicial:\n");
    for (int i = 0; i < 3; i++) {
        printf("  ");
        for (int j = 0; j < 3; j++) {
            if (G->grid[i][j] == 0) printf("_  ");
            else printf("%d  ", G->grid[i][j]);
        }
        printf("\n");
    }

    double t_start = get_time_s();
    node result = IDDFS(G, MAX_DEPTH);
    double elapsed = get_time_s() - t_start;

    if (result != NULL) {
        printf("\nSolucao encontrada!\n");
        printf("Passos : %d\n", solution_length(result));
        printf("Prof. usada: %d\n", result->pathCost);
        printf("Nos expand.: %ld\n", g_total_nodes);
        printf("Tempo : %.4fs\n", elapsed);

        // Libera a cadeia de nós da solução
        node aux = result;
        while (aux != NULL) {
            node prev = aux->parent;
            delNode(aux);
            aux = prev;
        }
    } else {
        printf("\nSolucao nao encontrada em %d niveis.\n", MAX_DEPTH);
        printf(" Nos expand.: %ld\n", g_total_nodes);
        printf(" Tempo : %.4fs\n", elapsed);
    }

    delGame(G);
    return 0;
}
