#include "bidirectional.h"
#include <stdio.h>
#include <stdlib.h>

// Função para imprimir o caminho quando o encontro ocorre
void printBidirectionalPath(node meetIn, node meetOut) {
    printf("Encontro de buscas detectado!\n");
}

node bidirectionalSearch(game* start, game* goal) {
    // Inicializando a fila
    fila frontierIn = newFila();
    fila frontierOut = newFila();

    // Criando os nós raiz (início e fim)
    node rootIn = childNode(NULL, 0);
    copyGame(start, rootIn->state);
    
    node rootOut = childNode(NULL, 0);
    copyGame(goal, rootOut->state);

    insert(frontierIn, rootIn);
    insert(frontierOut, rootOut);

    // Inicializamos a lista dos já visitados 
    fila exploredIn = newFila();
    fila exploredOut = newFila();
    insert(exploredIn, rootIn);
    insert(exploredOut, rootOut);

    while (frontierIn->first != NULL && frontierOut->first != NULL) {
        // EXPANSÃO DO INICIO
        node currentIn = pop(frontierIn);
        
        // Testa todos os movimentos (1 a 8 são as peças no 8-puzzle)
        for (int k = 1; k < 9; k++) {
            game* temp = newGame();
            copyGame(currentIn->state, temp);
            
            if (moveGame(temp, k)) {
                node child = childNode(currentIn, k);
                
                // Verifiquemos se o nó já foi visitado por outra busca
                if (isIn(exploredOut, child)) {
                    delGame(temp);
                    return child; // Encontrou :D
                }
                
                // Se ainda não está na lista, adicionamos ele
                if (!isIn(exploredIn, child)) {
                    insert(frontierIn, child);
                    insert(exploredIn, child);
                } else {
                    delNode(child);
                }
            }
            delGame(temp);
        }
        
        // EXPANSÃO DO FIM 
        node currentOut = pop(frontierOut);
        
        for (int k = 1; k < 9; k++) {
            game* temp = newGame();
            copyGame(currentOut->state, temp);
            
            if (moveGame(temp, k)) {
                node child = childNode(currentOut, k);
                
                // Verifiquemos se o nó já foi visitado por outra busca
                if (isIn(exploredIn, child)) {
                    delGame(temp);
                    printBidirectionalPath(NULL, child);
                    return child; // Encontrou XD
                }
                
                if (!isIn(exploredOut, child)) {
                    insert(frontierOut, child);
                    insert(exploredOut, child);
                } else {
                    delNode(child);
                }
            }
            delGame(temp);
    }

    return NULL;
}