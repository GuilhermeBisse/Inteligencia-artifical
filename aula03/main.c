#include "gradd.h"
#include "shill.h"
#include "genetic.h"

int main(){
    int tmax = 10000;
    double x_0 = 5;

    double* outQuad = geneticAlgorithm(quad,200,x_0,tmax);
    writeArray("outQuad.dat",outQuad,10000);

    double* outRast = geneticAlgorithm(rastrigin,200,x_0,tmax);
    writeArray("outRast.dat",outRast,10000);

    return 0;
}