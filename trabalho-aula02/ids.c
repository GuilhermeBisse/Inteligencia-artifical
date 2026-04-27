// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito
#include "game.h"
#include "tree.h"
#include <stdlib.h>

node DFS_Limited(node parent, int limit);

node IDS(game* G){

    // Cria o nó raiz (estado inicial)
    node root = childNode(NULL, 0);
    copyGame(G, root->state);

    // Começa com profundidade limite 0
    int limit = 0; 
    // Limite máximo de profundidade para evitar execução excessiva
    int maxLimit = 31; 

    while (limit <= maxLimit){

        // Executa DFS limitado com o limite atual
        node result = DFS_Limited(root, limit); 

        if (result != NULL){
            delNode(root);
            return result;
        }

        limit++;
    }
    
    delNode(root);
    return NULL; // Se não encontrar solução dentro do limite máximo
}