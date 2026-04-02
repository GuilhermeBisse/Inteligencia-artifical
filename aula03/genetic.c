#include "shill.h"
#include "math.h"
#include <stdlib.h>
#include <time.h>

double random_step(double x_0, double step){
    /*
        Passo aleatório de comprimento step.
    */
    return x_0+step*(2*(double)rand()/(double)RAND_MAX-1);
}

double best_in_population(double (*func)(double), double* pop, 
    int population_size){
    /*
        Procura pelo melhor elemento na população.
    */
    double best = pop[0];
    for (int i=1; i<population_size; i++){
        if (func(best)>func(pop[i]))
            best = pop[i];
    }
    return best;
}

int random_choice(double* pop, double (*func)(double), int population_size){
    /*
        Escolha aleatória de acordo com pesos determinados por func(pop)
    */
    //Normalização da probabilidade
    double Z = 0;
    for (int i=0; i<population_size; i++){
        Z += exp(-func(pop[i]));
    }
    //Número uniforme em (0,1)
    double r = (double)rand()/(double)RAND_MAX;
    //Propriedades do melhor
    int k = 0;
    double prob_k = exp(-func(pop[k]))/Z;
    //Retorna o índice k da menor probabilidade tal que prob[k]>=r
    for (int i=1; i<population_size; i++){
        double prob_i = exp(-func(pop[i]))/Z;
        if (prob_i>=r){
            if (prob_k>prob_i){
                k = i;
                prob_k = prob_i;
            }
        }
    }
    return k;
}

void swap_worse(double* pop, double (*func)(double), double candidate, 
    int population_size){
    /*
        Checa se candidate é melhor do que o pior em pop. Se sim, faz a troca.
    */
    int worse = 0;
    for (int i=1; i<population_size; i++){
        if (func(pop[worse])<func(pop[i]))
            worse = i;
    }
    if (func(pop[worse]<func(candidate)))
        pop[worse] = candidate;
}

double* geneticAlgorithm(double (*func)(double), int population_size, 
    double x_0, int tmax){

    //Variável de saída (trajetória do algoritmo)
    double* out = malloc(tmax*sizeof(double));
    out[0] = x_0;
    //Variável auxiliar para armazenar valor da função
    double auxf = func(x_0);
    //Variável auxiliar para armazenar população
    double old_population[population_size];
    double next_population[population_size];
    for (int i=0; i<population_size; i++){
        old_population[i] = random_step(x_0,1);
        next_population[i] = old_population[i];
    }
    //Variável auxiliar para armazenar nova população
    //Variável iterativa
    int t = 1;

    //Inicializa gerador de números aleatórios
    srand(time(NULL));

    //Procedimento iterativo para encontrar mínimo
    while (t<tmax){
        printf("%d \n", t);
        //Gera filhos de old_population
        for (int i=0; i<population_size; i++){
            int k_1 = random_choice(old_population,func,population_size);
            int k_2 = random_choice(old_population,func,population_size);
            //while (k_2==k_1){
            //    k_2 = random_choice(old_population,func,population_size);
            //}
            //Crossover (combinação)
            double aux = (old_population[k_1]+old_population[k_2])/2;
            //Mutação
            aux = random_step(aux,1);
            //Substitui pior candidato
            swap_worse(next_population,func,aux,population_size);
        }
        //Copia next_population para old_population
        for (int i=0; i<population_size; i++){
            old_population[i] = next_population[i];
        }
        //Armazena o melhor filho
        out[t] = best_in_population(func,old_population,population_size);
        //Incrementa t
        t++;
    }
    return out;
}