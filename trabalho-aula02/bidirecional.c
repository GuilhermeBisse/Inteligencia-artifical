#include "bidirecional.h"
#include <stdio.h>
#include <stdlib.h>

//Imprime o caminho da solução unindo as duas frentes de busca.
void printBidirectionalPath(node meetIn, node meetOut) {
    node stack[1000]; 
    int top = 0;

    node aux = meetIn;

    // Sobe do nó de encontro até a raiz (início) para armazenar o caminho
    while (aux != NULL) {
        stack[top++] = aux;
        aux = aux->parent;
    }

    printf("Caminho completo:\n");

    // Imprime do início até o nó de encontro (invertendo a pilha)
    for (int i = top - 1; i >= 0; i--) {
        printf("%d ", stack[i]->action);
    }

    // A partir daqui, percorre do nó de encontro até a meta
    // Nota: supõe-se que meetOut representa o mesmo estado, mas na árvore que partiu do fim
    aux = meetOut->parent;

    while (aux != NULL) {
        printf("%d ", aux->action);
        aux = aux->parent;
    }

    printf("\n");
}

//Busca Bidirecional (BDS)
//Expande duas frentes simultaneamente: uma partindo de 'start' (inico) e outra de 'goal' (final/objetivo)
node bidirectionalSearch(game* start, game* goal) {

    // Inicializa as filas de fronteira e visitados para ambos os lados
    fila frontierIn = newFila();
    fila frontierOut = newFila();

    fila exploredIn = newFila();
    fila exploredOut = newFila();

    // Cria os nós raiz para cada lado da busca
    node rootIn = childNode(NULL, 0);
    copyGame(start, rootIn->state);

    node rootOut = childNode(NULL, 0);
    copyGame(goal, rootOut->state);

    // Insere as raízes nas respectivas fronteiras e conjuntos de explorados
    insert(frontierIn, rootIn);
    insert(frontierOut, rootOut);

    insert(exploredIn, rootIn);
    insert(exploredOut, rootOut);

    // Continua enquanto houver estados para explorar em ambas as frentes 
    while (frontierIn->first != NULL && frontierOut->first != NULL) {

        //Lado do Inicio======================================================================================
        node currentIn = pop(frontierIn);

        // Tenta mover cada peça de 1 a 8 para gerar novos estados
        for (int k = 1; k < 9; k++) {
            game* temp = newGame();
            copyGame(currentIn->state, temp);

            // Se o movimento da peça k for válido
            if (moveGame(temp, k)) {

                node child = childNode(currentIn, k);
                copyGame(temp, child->state);

                // Verifica se este novo estado já foi visitado pela busca que vem do ponto final
                if (isIn(exploredOut, child)) {
                    delGame(temp);
                    return child; // Encontrou!
                }

                // Se for um estado novo para este lado, adiciona à fila 
                if (!isIn(exploredIn, child)) {
                    insert(frontierIn, child);
                    insert(exploredIn, child);
                } else {
                    delNode(child); // Libera memória se o estado já foi visto por este lado
                }
            }
            delGame(temp);
        }

        // Lado do Fim=====================================================================================
        node currentOut = pop(frontierOut);

        for (int k = 1; k < 9; k++) {
            game* temp = newGame();
            copyGame(currentOut->state, temp);

            if (moveGame(temp, k)) {

                node child = childNode(currentOut, k);
                copyGame(temp, child->state);

                // Verifica se este estado gerado a partir do fim já foi visto pela busca que partiu do início
                if (isIn(exploredIn, child)) {
                    delGame(temp);
                    return child; // Match! <3
                }

                // Se for um estado novo para este lado, adiciona à fila 
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

    return NULL; // Não há conexão entre os estados no espaço de busca
}
