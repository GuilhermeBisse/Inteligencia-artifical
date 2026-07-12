// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito


#include "env.h"
#include "agent.h"

#ifndef KNOWLEDGE_H
#define KNOWLEDGE_H

/*
    Base de conhecimento do agente.

    visited[i][j]   : o agente já esteve na posição (i,j) (portanto já sabe com certeza se há buraco/monstro ali, pois observou
    diretamente ao chegar).
    noBuraco[i][j]  : foi inferido (via ausência de vento numa vizinhança visitada) que (i,j) não pode ter buraco.
    noMonstro[i][j] : foi inferido (via ausência de cheiro numa vizinhança visitada) que (i,j) não pode ter monstro.

    Regra de inferência central (Modus Tollens): se um local visitado P não sente vento, então, para todo vizinho
    V de P, ¬vento(P) ⟹ ¬buraco(V). Racional análogo para cheiro/monstro.
*/
typedef struct {
    int h, w;
    bool** visited;
    bool** noBuraco;
    bool** noMonstro;
} KB;

KB newKB(int h, int w);
void delKB(KB* kb);
void updateKB(KB* kb, place* p);
bool isFullySafe(KB kb, int i, int j);
bool isBuracoSafe(KB kb, int i, int j);
place* chooseTarget(KB kb, enviroment E, agent A, bool wantExit);

#endif
