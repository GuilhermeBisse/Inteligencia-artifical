// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

double* arrayMul(double* u, double* v, int len){
    double *out = malloc(len*sizeof(double));
    assert(out);
    for (int i=0; i<len; i++){
        out[i] = u[i]*v[i];
    }
    return out;
}

double* arraySum(double* u, double* v, int len){
    double *out = malloc(len*sizeof(double));
    assert(out);
    for (int i=0; i<len; i++){
        out[i] = u[i]+v[i];
    }
    return out;
}

double* arraySub(double* u, double* v, int len){
    double *out = malloc(len*sizeof(double));
    assert(out);
    for (int i=0; i<len; i++){
        out[i] = u[i]-v[i];
    }
    return out;
}

void copyArray(double* to, double* from, int len){
    for (int i=0; i<len; i++)
        to[i] = from[i];
}

double norm(double* v, int len){
    double out = 0;
    for (int i=0; i<len; i++)
        out += v[i]*v[i];
    return sqrt(out);    
}