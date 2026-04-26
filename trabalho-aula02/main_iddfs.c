/*Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito
 *
 * 8-Puzzle — Busca em Profundidade com Aprofundamento Iterativo (IDDFS)
 *
 * Realiza 10 experimentos com configuracoes iniciais aleatorias e
 * soluveis, mostrando em cada um:
 *   - Estado inicial
 *   - Numero de passos da solucao
 *   - Profundidade em que foi encontrada
 *   - Total de nos expandidos
 *   - Tempo de execucao
 *
 * No final, exibe media e desvio padrao de cada solucao.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "game.h"
#include "tree.h"
#include "iddfs.h"

#define N_EXPERIMENTS 10
#define MAX_DEPTH     31


#ifdef _WIN32
#  include <windows.h>
static double get_time_s(void) {
    LARGE_INTEGER freq, cnt;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&cnt);
    return (double)cnt.QuadPart / (double)freq.QuadPart;
}
#else
#  include <time.h>
static double get_time_s(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}
#endif

/* Utilitarios: configuracao inicial soluvel */

/*
 * Conta o numero de inversoes no vetor flat (excluindo o zero).
 * Uma configuracao do 8-puzzle e soluvel sse o nr de inversoes e par
 */
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

/*
 * Gera uma configuracao inicial aleatoria e soluvel (diferente da meta)
 */
static void generate_solvable(game* G) {
    int flat[9];
    do {
        /* Preenche [0..8] e embaralha com Fisher-Yates */
        for (int i = 0; i < 9; i++) flat[i] = i;
        for (int i = 8; i > 0; i--) {
            int j = rand() % (i + 1);
            int tmp = flat[i]; flat[i] = flat[j]; flat[j] = tmp;
        }
    } while (count_inversions(flat) % 2 != 0   /* nao soluvel */
          || (flat[0]==0 && flat[1]==1 && flat[2]==2 &&
              flat[3]==3 && flat[4]==4 && flat[5]==5 &&
              flat[6]==6 && flat[7]==7 && flat[8]==8)); /* ja e meta */

    /* Copia para o grid do jogo */
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            G->grid[i][j] = flat[i * 3 + j];
}


/* Utilitarios: impressao */

static void print_separator(void) {
    printf("  ");
    for (int i = 0; i < 58; i++) putchar('-');
    putchar('\n');
}

static void print_header(void) {
    printf("\n");
    for (int i = 0; i < 62; i++) putchar('=');
    printf("\n  8-PUZZLE - Aprofundamento Iterativo (IDDFS)\n");
    printf("  Prof. maxima: %d  |  Experimentos: %d\n", MAX_DEPTH, N_EXPERIMENTS);
    for (int i = 0; i < 62; i++) putchar('=');
    printf("\n");
}

/* Conta passos percorrendo a cadeia pai->filho */
static int solution_length(node nd) {
    int steps = 0;
    for (node aux = nd; aux->parent != NULL; aux = aux->parent)
        steps++;
    return steps;
}

/* main */


int main(void) {
    print_header();

    /* Inicializa gerador de numeros aleatorios */
    srand((unsigned int)time(NULL));

    /* Metricas de cada experimento */
    int    steps_arr[N_EXPERIMENTS];
    int    depth_arr[N_EXPERIMENTS];
    long   nodes_arr[N_EXPERIMENTS];
    double time_arr [N_EXPERIMENTS];
    int    solved = 0;

    for (int exp = 1; exp <= N_EXPERIMENTS; exp++) {
        printf("\n[Experimento %02d]\n", exp);

        /* Gera estado inicial soluvel */
        game* G = newGame();
        generate_solvable(G);

        printf("  Estado inicial:\n");
        for (int i = 0; i < 3; i++) {
            printf("    ");
            for (int j = 0; j < 3; j++) {
                if (G->grid[i][j] == 0)
                    printf("_  ");
                else
                    printf("%d  ", G->grid[i][j]);
            }
            printf("\n");
        }

        /* Executa IDDFS e mede tempo */
        double t_start = get_time_s();
        node result = IDDFS(G, MAX_DEPTH);
        double elapsed = get_time_s() - t_start;

        if (result != NULL) {
            int steps = solution_length(result);
            int depth = result->pathCost;

            printf("  Solucao encontrada!\n");
            printf("  Passos     : %d\n",   steps);
            printf("  Prof. usada: %d\n",   depth);
            printf("  Nos expand.: %ld\n",  g_total_nodes);
            printf("  Tempo      : %.4fs\n", elapsed);

            steps_arr[exp-1] = steps;
            depth_arr[exp-1] = depth;
            nodes_arr[exp-1] = g_total_nodes;
            time_arr [exp-1] = elapsed;
            solved++;

            /* Libera nos da solucao */
            node aux = result;
            while (aux != NULL) {
                node prev = aux->parent;
                delNode(aux);
                aux = prev;
            }
        } else {
            printf("  Solucao nao encontrada em %d niveis.\n", MAX_DEPTH);
            printf("  Nos expand.: %ld\n",  g_total_nodes);
            printf("  Tempo      : %.4fs\n", elapsed);

            steps_arr[exp-1] = -1;
            depth_arr[exp-1] = MAX_DEPTH;
            nodes_arr[exp-1] = g_total_nodes;
            time_arr [exp-1] = elapsed;
        }

        delGame(G);
    }
    return 0;
}
