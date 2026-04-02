#include "shill.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

double quad(double x){
    return x*x;
}

double rastrigin(double x){
    return 10+x*x-10*cos(2*PI*x);
}

double* descidaEstocastica(double (*func)(double), double x_0, double eta,
    int tmax){

    //Variável de saída (trajetória do algoritmo)
    double* out = malloc(tmax*sizeof(double));
    out[0] = x_0;
    //Variável auxiliar para armazenar valor da função
    double auxf = func(x_0);
    //Variável auxiliar para armazenar posição
    double auxx = x_0;
    //Variável iterativa
    int t = 1;

    //Inicializa gerador de números aleatórios
    srand(time(NULL));

    //Procedimento iterativo para encontrar mínimo
    while (t<tmax){
        //Determina passo aleatório
        double delta = eta*(2*((double)rand())/((double)(RAND_MAX))-1);
        //Deslocamento
        x_0 += delta;
        //Critério de aceitação do novo passo
        if (auxf<func(x_0)){
            //Probabilidade de aceitação
            double p = exp(auxf-func(x_0));
            double r = (double)rand()/(double)RAND_MAX;
            //Rejeição
            if (r>p)
                x_0 = out[t-1];
        }
        auxf = func(x_0);
        out[t] = x_0;

        //Incrementa t
        t++;
    }

    //Preenche out com o último valor antes de parar o procedimento iterativo
    for (int i=t; i<tmax; i++)
        out[i] = out[t-1];

    return out;
}

// Basicamente o algoritmo do recozimento simulado é o mesmo da descida estocástica. 
// A única diferença é a adição do parâmetro beta (inverso da Temperatura), além do parâmetro de controle do beta (fator_beta) e o incremento_t, que serve para controlar a frequência de atualização do beta. (Exemplo: incremento_t = 3, o beta irá atualizar com beta = beta*fator_beta a cada 3 iterações)
// O recozimento simulado é um algoritmo capaz de escapar de mínimos locais, diferente da descida estocástica, já que faz uso do parâmetro beta como uma espécie de tolerância para aceitar soluções piores. Com um beta alto (apontando pro infinito), o algoritmo se torna mais "fresco", já que exp(-infinito) é 0. Com um beta apontando para 0, o algoritmo se torna mais tolerante, já que exp(0) é 1.
// Sobre a questão de domínio contínuo, esse algoritmo já lida com problemas no domínio contínuo, usando double em vez de int e calculando o deslocamento (delta) como um número aleatório, usando o parâmetro eta para controlar o escopo do deslocamento.

double* recozimentoSimulado(double (*func)(double), double x_0, double eta,
    int tmax, double beta_0, double fator_beta, int incremento_t){

    //Variável de saída (trajetória do algoritmo)
    double* out = malloc(tmax*sizeof(double));
    out[0] = x_0;
    //Variável auxiliar para armazenar valor da função
    double auxf = func(x_0);
    //Variável auxiliar para armazenar posição
    double auxx = x_0;
    //Variável iterativa
    int t = 1;

    double beta = beta_0;

    //Inicializa gerador de números aleatórios
    srand(time(NULL));

    //Procedimento iterativo para encontrar mínimo
    while (t<tmax){
        //Determina passo aleatório
        double delta = eta*(2*((double)rand())/((double)(RAND_MAX))-1);
        //Deslocamento
        x_0 += delta;

        //Critério de aceitação do novo passo
        if (auxf<func(x_0)){
            //Probabilidade de aceitação
            double p = exp(beta*(auxf-func(x_0)));
            double r = (double)rand()/(double)RAND_MAX;
            //Rejeição
            if (r>p)
                x_0 = out[t-1];
        }
        auxf = func(x_0);
        out[t] = x_0;

        if (t % incremento_t == 0){
            beta = beta * fator_beta;
        }

        //Incrementa t
        t++;
    }


    //Preenche out com o último valor antes de parar o procedimento iterativo
    for (int i=t; i<tmax; i++)
        out[i] = out[t-1];

    return out;
}