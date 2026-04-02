// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito

#include <stdio.h>

#define PI 3.141592684

double* descidaEstocastica(double (*func)(double), double x_0, double eta,
    int tmax);

// Adição do algoritmo do recozimento simulado, que é uma variação da descida estocástica. Está melhor explicado no arquivo shill.c.
double* recozimentoSimulado(double (*func)(double), double x_0, double eta,
    int tmax, double beta_0, double fator_beta, int incremento_t);
    
double quad(double x);
double rastrigin(double x);