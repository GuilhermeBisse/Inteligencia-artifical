// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito

#include "swarm.h"
#include "particles.h"
#include "utils.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>


void initSwarm(swarm* S, int N, int dim, double* c1, double* c2){
/*
    Inicializa um enxame com posições aleatórias para as partículas.
    N: número de partículas
    dim: dimensão do espaço de busca
*/
    S->P = malloc(N*sizeof(particle));
    assert(S->P);
    srand(time(NULL));
    for (int i=0; i<N; i++){
        double aux_x[dim];
        for (int s=0; s<dim; s++){
            aux_x[s] = ((double)rand())/((double)RAND_MAX);
            aux_x[s] = 2*aux_x[s]-1;
        }
        initParticle(&S->P[i],dim,aux_x);
    }
    //Posição ótima inicial é também aleatória
    S->x_opt = malloc(dim*sizeof(double));
    assert(S->x_opt);
    for (int s=0; s<dim; s++){
        S->x_opt[s] = ((double)rand())/((double)(RAND_MAX));
        S->x_opt[s] = 2*S->x_opt[s]-1;
    }
    //Dimensão
    S->dim = dim;
    //Vetores coeficientes
    S->c1 = c1;
    S->c2 = c2;
    S->N = N;
}

void freeSwarm(swarm* S){
    if (S!=NULL){
        for (int i=0; i<S->N; i++){
            freeParticle(&S->P[i]);
        }
        free(S->P);
    }
}

// Aqui nós apenas adicionamos os parâmetros de inércia w e de velocidade máxima vmax, para que a função updateVelocity() funcione de forma adequada. Importante ressaltar que o cálculo de w e vmax é feito na função main.c e passado para a função updateSwarm(), mudando a inércia a cada iteração e mantendo a velocidade máxima.
void updateSwarm(swarm* S, double (*cost)(double*), double w, double vmax){
    //Valor ótimo atual
    double f_opt = cost(S->x_opt);
    //Índice inicial da melhor partícula é -1
    int id_opt = -1;

    //Atualiza posições das partículas e busca novo valor ótimo
    for (int i=0; i<S->N; i++){
        updateVelocity(&S->P[i],S->c1,S->c2,S->x_opt, w, vmax);
        updatePosition(&S->P[i],cost);
        double f_new = cost(S->P[i].x_opt);
        if (f_opt>f_new){
            f_opt = f_new;
            id_opt = i;
        }
    }

    //Se melhorou ótimo atual, atualiza
    if (id_opt>-1){
        copyArray(S->x_opt,S->P[id_opt].x_opt,S->dim);
    }
}