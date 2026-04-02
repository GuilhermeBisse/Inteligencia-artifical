/* Busca Local por Feixes
 *
 * Objetivo: maximizar f(x) = -(x^2) + 10*sin(x) + 50
 * no intervalo [-10, 10]
 *
 * O algoritmo mantém k estados simultaneamente.
 * A cada iteração, gera vizinhos de todos os estados do feixe,
 * avalia todos eles e seleciona os k melhores para a próxima iteração.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define K 5           // tamanho do feixe
#define MAX_ITER 200
#define N_VIZ 10      // vizinhos por estado
#define PASSO 0.3


// Função objetivo a ser maximizada
double f(double x) {
    return -(x*x) + 10*sin(x) + 50;
}


//Gera um double aleatório
double rand_range(double min, double max) {
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

typedef struct {
    double x;
    double fx;
} Estado;

// comparador pra qsort (decrescente)
int cmp(const void *a, const void *b) {
    Estado *ea = (Estado *)a;
    Estado *eb = (Estado *)b;
    if (eb->fx > ea->fx) return 1;
    if (eb->fx < ea->fx) return -1;
    return 0;
}

int main() {
    srand(time(NULL));

    Estado feixe[K];
    Estado candidatos[K * N_VIZ];

    // inicializa o feixe com estados aleatorios
    for (int i = 0; i < K; i++) {
        feixe[i].x = rand_range(-10, 10);
        feixe[i].fx = f(feixe[i].x);
    }

    printf("feixe inicial:\n");
    for (int i = 0; i < K; i++)
        printf("x=%.4f f(x)=%.4f\n", feixe[i].x, feixe[i].fx);
    printf("\n");

    for (int iter = 0; iter < MAX_ITER; iter++) {
        // gera vizinhos de cada estado
        int idx = 0;
        for (int i = 0; i < K; i++) {
            for (int j = 0; j < N_VIZ; j++) {
                double nx = feixe[i].x + rand_range(-PASSO, PASSO);
                // clamp
                if (nx < -10) nx = -10;
                if (nx >  10) nx = 10;
                candidatos[idx].x = nx;
                candidatos[idx].fx = f(nx);
                idx++;
            }
        }

        // pega os K melhores
        qsort(candidatos, K * N_VIZ, sizeof(Estado), cmp);
        for (int i = 0; i < K; i++)
            feixe[i] = candidatos[i];
    }

    printf("melhor resultado encontrado:\n");
    printf("  x = %.6f\n", feixe[0].x);
    printf("  f(x) = %.6f\n", feixe[0].fx);

    return 0;
}