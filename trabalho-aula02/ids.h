// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito
#ifndef IDS_H
#define IDS_H

#include "game.h"
#include "tree.h"

// Declaração da função IDS (Iterative Deepening Search)
// Executa múltiplas buscas DFS_Limited com profundidade crescente, combinando a eficiência de memória do DFS com a completude do BFS.

node IDS(game* G);

#endif