// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito
#ifndef DFS_LIMITED_H
#define DFS_LIMITED_H

#include "tree.h"

// Declaração da função DFS_Limited
// Implementa a busca em profundidade com limite de profundidade, evitando exploração infinita no espaço de estados.
// É utilizada diretamente e também como base para o algoritmo IDS.
node DFS_Limited(node parent, int limit);

#endif