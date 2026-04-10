// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito

#ifndef UTILS_H
#define UTILS_H

double* arrayMul(double* u, double* v, int len);
double* arraySum(double* u, double* v, int len);
double* arraySub(double* u, double* v, int len);
void copyArray(double* to, double* from, int len);
double norm(double* v, int len);

#endif