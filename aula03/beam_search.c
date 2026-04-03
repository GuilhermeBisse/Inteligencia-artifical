// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define K 5
#define MAX_ITER 200
#define N_VIZ 10
#define PASSO 0.3

// Função f(x) que cria o terreno. O algoritmo vai ser responsável por descobir o valor de x que atinge o máximo global (onde f(x) é maior)
double f(double x) {
    return -(x * x) + 10 * sin(x) + 50;
}

// Essa função gera valores aleatórios em ponto flutuante dentro de um intervalo especificado pelos parâmetros min e max, servindo para posicionar os exploradores.
double rand_range(double min, double max) {
    double r = (double)rand() / (double)RAND_MAX;
    return min + r * (max - min);
}

typedef struct {
    double x;
    double fx;
} Estado;

int cmp(const void *a, const void *b) {
    Estado *e1 = (Estado *)a;
    Estado *e2 = (Estado *)b;
    
    if (e1->fx < e2->fx) {
        return 1;
    } else if (e1->fx > e2->fx) {
        return -1;
    } else {
        return 0;
    }
}

// Esse é o código de implementação do algoritmo de busca local por feixes.
// Basicamente, o vetor feixe[K] armazena cada um dos exploradores (no nosso caso, 5 exploradores) que são inicializados em posições aleatórias do terreno.
// Já o vetor candidatos[total_vizinhos] armazena todas os pontos que os exploradores. (No nosso caso, 5 exploradores com N_VIZ=10.
// Ou seja, cada explorador dá 10 passos em volta de si mesmo (por isso total_vizinhos = 50).
// No loop do algoritmo, cada explorador dá seus 10 passos, que são armazenados no vetor candidatos.
// Quando todos terminaram, esse vetor candidatos é ordenado usando o qsort(), e os 5 primeiros valores (os 5 melhores), são armazenados no vetor feixe.
// Esse processo iterativo se repete 200 vezes (MAX_ITER = 200)
// Esse código implementou o local_beam_search considerando o escopo do domínio contínuo, com valores em ponto flutuante (double), além de uso da função
// rand_range() para gerar valores aleatórios não discretos (somente inteiros).

Estado local_beam_search() {
    Estado feixe[K];
    int total_vizinhos = K * N_VIZ;
    Estado candidatos[total_vizinhos];

    for (int i = 0; i < K; i++) {
        feixe[i].x = rand_range(-10.0, 10.0);
        feixe[i].fx = f(feixe[i].x);
    }

    for (int iteracao = 0; iteracao < MAX_ITER; iteracao++) {
        int cont = 0;
        
        for (int i = 0; i < K; i++) {
            for (int j = 0; j < N_VIZ; j++) {
                double novo_x = feixe[i].x + rand_range(-PASSO, PASSO);
                
                if (novo_x < -10.0) {
                    novo_x = -10.0;
                }
                if (novo_x > 10.0) {
                    novo_x = 10.0;
                }
                
                candidatos[cont].x = novo_x;
                candidatos[cont].fx = f(novo_x);
                cont++;
            }
        }

        qsort(candidatos, total_vizinhos, sizeof(Estado), cmp);
        
        for (int i = 0; i < K; i++) {
            feixe[i] = candidatos[i];
        }
    }

    return feixe[0];
}
