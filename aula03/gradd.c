#include "gradd.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double quadradoTeste(double x){
    return 2*x;
}

double rastriginTeste(double x){
    double pi = acos(-1);
    return 2*(x+10*pi*sin(2*pi*x));
}

double* descidaGradiente(double (*grad)(double), double x_0, double eta,
    int tmax, double eps){

    //Variável de saída (trajetória do algoritmo)
    double* out = malloc(tmax*sizeof(double));
    out[0] = x_0;
    //Variável auxiliar para armazenar gradiente
    double auxGrad = grad(x_0);
    //Variável iterativa
    int t = 1;

    //Procedimento iterativo para encontrar mínimo (máximo se eta<0)
    while (t<tmax && fabs(auxGrad)>eps){
        out[t] = out[t-1]-eta*auxGrad;
        auxGrad = grad(out[t]);
        t++;
    }

    //Preenche out com o último valor antes de parar o procedimento iterativo
    for (int i=t; i<tmax; i++)
        out[i] = out[t-1];

    return out;
}

void writeArray(char* fname, double* out, int n){
    FILE* fp = fopen(fname,"w");
    for (int i=0; i<n; i++){
        fprintf(fp,"%f \n",out[i]);
    }
    fclose(fp);
}