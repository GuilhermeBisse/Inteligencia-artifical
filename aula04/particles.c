// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "particles.h"
#include "utils.h"

void initParticle(particle* p, int dim, double* x_0){
/*
    Inicializa uma partícula.
    p: partícular a ser inicializada
    dim: dimensão do espaço de busca
    x_0: posição inicial
*/
    //Posição inicial
    p->x = malloc(dim*sizeof(double));
    assert(p->x);
    copyArray(p->x,x_0,dim);
    //Posição ótima
    p->x_opt = malloc(dim*sizeof(double));
    assert(p->x_opt);
    copyArray(p->x_opt,x_0,dim);
    //Velocidade inicial é zero
    p->v = malloc(dim*sizeof(double));
    assert(p->v);
    for (int i=0; i<dim; i++)
        p->v[i] = 0;
    //Dimensão
    p->dim = dim;
}

void freeParticle(particle* p){
    if (p!=NULL){
        if (p->x!=NULL)
            free(p->x);
        if (p->x_opt!=NULL)
            free(p->x_opt);
        if (p->v!=NULL)
            free(p->v);
    }
}

// Aqui nessa função foi onde fizemos a maioria das modificações, adicionando ambos os métodos pedidos pelo professor no enunciado da atividade.
// Usamos o método da inércia, onde multiplicamos o vetor de velocidade da partículo por um fator de inércia w, que varia conforme a iteração do programa, tendo seu máximo em wmax = 0.9 e seu mínimo em wmin = 0.4, como sugerido nos slides. Esse método é descrito no slide "Controle de velocidade II".
// Usamos a operação descrita em (4) para o cálculo da velocidade em vi[t+1]. A operação (5) foi implementada na função main.c, para o cálculo da inércia w naquela iteração t.
// O outro método usado foi o método de limitação da velocidade usando a normalização do vetor. Esse método apareceu no slide "Controle de velocidade I".
// Seguimos a operação (3) do slide para limitar a velocidade.

void updateVelocity(particle* p, double* c1, double* c2, double* swarm_opt, double w, double vmax){
/*
    Função para atualizar a velocidade de uma partícula.
    p: partícula a ser atualizada
    c1 e c2: vetores de coeficientes
    swarm_opt: vetor ótimo do enxame
*/
    //Termo individual da velocidade
    double* aux_sub1 = arraySub(p->x_opt,p->x,p->dim);
    double* aux_mul1 = arrayMul(c1,aux_sub1,p->dim);
    //Termo coletivo da velocidade
    double* aux_sub2 = arraySub(swarm_opt,p->x,p->dim);
    double* aux_mul2 = arrayMul(c2,aux_sub2,p->dim);
    //Ponteiro para velocidade anterior
    double* aux_v = p->v;

    // Aqui implementamos o método da inércia (4), ao multiplicar o vetor da velocidade anterior pelo fator de inércia, e somar com a multiplicação dos termos individual e coletivo, seguindo a operação (4) do slide "Controle de velocidade II".
    double* vetor_w = malloc(p->dim * sizeof(double));
    assert(vetor_w);
    for (int i=0; i<p->dim; i++){
        vetor_w[i] = w*aux_v[i];
    }
    //Atualiza velocidade
    double* aux_sum = arraySum(aux_mul1,aux_mul2,p->dim);
    p->v = arraySum(vetor_w,aux_sum,p->dim);

    // Aqui implementamos o método de limitação da velocidade usando a normalização do vetor, seguindo a operação (3) do slide "Controle de velocidade I". Calculamos a norma do vetor de velocidade, e se ela for maior que vmax, normalizamos o vetor e multiplicamos por vmax para limitar a velocidade. Para o cálculo da norma, aproveitamos a função norm(), que já estava pronta nos arquivos utils.h e utils.c.
    double vetor_normalizado = norm(p->v, p->dim);
    if (vetor_normalizado > vmax && vetor_normalizado > 0) {
        for (int i=0; i<p->dim; i++) {
            p->v[i] = vmax * (p->v[i] / vetor_normalizado);
        }
    }


    //Libera memória
    free(aux_sub1);
    free(aux_sub2);
    free(aux_mul1);
    free(aux_mul2);
    free(aux_v);
    free(aux_sum);
    free(vetor_w);
}

void updatePosition(particle* p, double (*cost)(double*)){
/*
    Função para atualiar a posição de uma parícula.
    p: partícula a ser atualizada
    cost: função custo
*/
    //Ponteiro para posição anterior
    double* aux_x = p->x;
    //Atualiza posição atual
    p->x = arraySum(aux_x,p->v,p->dim);
    //Checa se x_opt deve ser atualizada
    double cost_prev = cost(p->x_opt);
    double cost_new = cost(p->x);
    if (cost_prev>cost_new){
        copyArray(p->x_opt,p->x,p->dim);
    }
    //Libera memória
    free(aux_x);
}