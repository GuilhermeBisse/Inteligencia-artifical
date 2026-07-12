// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito


#include "env.h"

#ifndef AGENT_H
#define AGENT_H

typedef struct {
    place* onde;
    bool comOuro;
    bool temFlecha;
    int score;
}agent;

extern bool agentVerbose; // controla os prints internos de move() (1=imprime, 0=silencioso)

void printSimulation(agent A, enviroment E);
bool move(agent* A, enviroment E, place* target);
agent newAgent(enviroment E);
void sense(agent A);

#endif 