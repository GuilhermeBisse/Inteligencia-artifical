// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito

#include <stdlib.h>
#include <stdio.h>
#include "particles.h"

#ifndef SWARM_H
#define SWARM_H

typedef struct{
    int N; //número de partículas
    particle* P; //conjunto de partículas
    int dim; //dimensão
    double* x_opt; //posição ótima
    //Vetores-coeficientes
    double* c1;
    double* c2;
} swarm;

void initSwarm(swarm* S, int N, int dim, double* c1, double* c2);
void freeSwarm(swarm* S);
void updateSwarm(swarm* S, double (*cost)(double*), double w, double vmax);

#endif