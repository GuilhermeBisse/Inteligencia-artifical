// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito

#ifndef PARTICLES_H
#define PARTICLES_H

typedef struct {
    double* x; //posição atual
    double* x_opt; //posição ótima
    double* v; //velocidade
    int dim; //dimensão
} particle;

void initParticle(particle* p, int dim, double* x_0);
void freeParticle(particle* p);
void updateVelocity(particle* p, double* c1, double* c2, double* swarm_opt, double w, double vmax);
void updatePosition(particle* p, double (*cost)(double*));

#endif