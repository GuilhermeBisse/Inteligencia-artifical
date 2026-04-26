
#include "bidirecional.h"
#include <stdio.h>
#include <stdlib.h>

void printBidirectionalPath(node meetIn, node meetOut) {
    node stack[1000];
    int top = 0;

    node aux = meetIn;

    while (aux != NULL) {
        stack[top++] = aux;
        aux = aux->parent;
    }

    printf("Caminho completo:\n");

    for (int i = top - 1; i >= 0; i--) {
        printf("%d ", stack[i]->action);
    }

    aux = meetOut->parent;

    while (aux != NULL) {
        printf("%d ", aux->action);
        aux = aux->parent;
    }

    printf("\n");
}

node bidirectionalSearch(game* start, game* goal) {

    fila frontierIn = newFila();
    fila frontierOut = newFila();

    fila exploredIn = newFila();
    fila exploredOut = newFila();

    node rootIn = childNode(NULL, 0);
    copyGame(start, rootIn->state);

    node rootOut = childNode(NULL, 0);
    copyGame(goal, rootOut->state);

    insert(frontierIn, rootIn);
    insert(frontierOut, rootOut);

    insert(exploredIn, rootIn);
    insert(exploredOut, rootOut);

    while (frontierIn->first != NULL && frontierOut->first != NULL) {

        // lado do início
        node currentIn = pop(frontierIn);

        for (int k = 1; k < 9; k++) {
            game* temp = newGame();
            copyGame(currentIn->state, temp);

            if (moveGame(temp, k)) {

                node child = childNode(currentIn, k);
                copyGame(temp, child->state);

                // encontrou estado já visitado pelo outro lado
                if (isIn(exploredOut, child)) {
                    delGame(temp);
                    return child;
                }

                if (!isIn(exploredIn, child)) {
                    insert(frontierIn, child);
                    insert(exploredIn, child);
                } else {
                    delNode(child);
                }
            }

            delGame(temp);
        }

        // lado do objetivo
        node currentOut = pop(frontierOut);

        for (int k = 1; k < 9; k++) {
            game* temp = newGame();
            copyGame(currentOut->state, temp);

            if (moveGame(temp, k)) {

                node child = childNode(currentOut, k);
                copyGame(temp, child->state);

                if (isIn(exploredIn, child)) {
                    delGame(temp);
                    return child;
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
    }

    return NULL;
}
