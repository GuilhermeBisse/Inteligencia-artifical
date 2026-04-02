#ifndef GRADD_H
#define GRADD_H

double quadradoTeste(double x);
double rastriginTeste(double x);

double* descidaGradiente(double (*grad)(double), double x_0, double eta,
    int tmax, double eps);

void writeArray(char* fname, double* out, int n);

#endif