#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "bidirecional.h"

#define MAX_NODES 10000

// Inicializa a fila
void initFila(Fila* fila) {
    fila->frente = fila->tras = -1;
}

// Verifica se a fila está vazia
bool isVazio(Fila* fila) {
    return fila->tras == -1;
}

// Insere elemento na fila
void push(Fila* fila, Node* value) {
    if (fila->tras == MAX_NODES - 1) 
        return;
    if (fila->frente == -1) 
        fila->frente = 0;
    // Avança a traseira da fila (+1) e adiciona o novo valor/nó neste novo espaço
    fila->items[++fila->tras] = value;
}

// Remove elemento da fila
Node* pop(Fila* fila) {
    // Checa se está vazio
    if (isVazio(fila)) 
        return NULL;
    // O nó "item" vai conter o nó removido da frete da fila (pop!)
    Node* item = fila->items[fila->frente++];
    if (fila->frente > fila->tras) 
        fila->frente = fila->tras = -1; 
    return item;
}


//=======================COMPARAÇÃO DE ESTADOS=======================
// Verifica se dois estados são iguais 
bool isEqual(Estado a, Estado b) {
    for (int i = 0; i < 9; i++) {
        if (a.tabuleiro[i] != b.tabuleiro[i]) 
            return false;
    }
    return true; 
}


//=======================GERAR VIZINHOS=======================
// Gera todos os estados possíveis a partir de um estado atual
// Retorna quantos vizinhos foram gerados (máx 4)
int getNeighbors(Estado s, Estado neighbors[]) {

    int pos;

    // Encontra posição do zero (espaço vazio)
    for (int i = 0; i < 9; i++) {
        if (s.tabuleiro[i] == 0) 
            pos = i;
    }

    // Localiza em qual linha e coluna o espaço vazio se encontra 
    int row = pos / 3;
    int col = pos % 3;

    // Número de movimentos encontrados
    int count = 0;

    // Movimentos possíveis: cima, baixo, esquerda, direita 
    int moves[4][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1} // O primeiro elemento de cada chave vai mover as linhas, 
    };                                   // enquando o segundo move as colunas

    // O for abaixo substitui o uso de de ifs para avançar ou retroceder linhas ou colunas
    for (int i = 0; i < 4; i++) {

        int newRow = row + moves[i][0];  
        int newCol = col + moves[i][1]; 

        // Verifica se movimento é válido (dentro do tabuleiro)
        if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3) {

            Estado newEstado = s;
            
            int newPos = newRow * 3 + newCol;

            // Troca o zero com a posição válida
            newEstado.tabuleiro[pos] = newEstado.tabuleiro[newPos];
            newEstado.tabuleiro[newPos] = 0;

            neighbors[count++] = newEstado;
        }
    }

    return count;
}


//=======================VERIFICA SE JÁ FOI VISITADO=======================
bool contains(Estado s, Node* visited[], int size) {
    for (int i = 0; i < size; i++) {
        // Compara o estado s com o vetor de estados já visitados
        if (isEqual(s, visited[i]->estado)) 
            return true;
    }
    return false;
}


//=======================ENCONTRAR INTERSEÇÃO=======================
// Verifica se os estados já visitados das buscas 1 e 2 se encontraram (size1 e size2 são o tamanho de cada vetor)
Node* findIntersection(Node* visited1[], int size1,
                       Node* visited2[], int size2) {

    for (int i = 0; i < size1; i++) {
        for (int j = 0; j < size2; j++) {
            if (isEqual(visited1[i]->estado, visited2[j]->estado)) {
                return visited1[i];
            }
        }
    }
    return NULL;
}


//=======================IMPRIMIR ESTADO=======================

void printEstado(Estado s) {
    for (int i = 0; i < 9; i++) {
        printf("%d ", s.tabuleiro[i]);
        if ((i + 1) % 3 == 0) printf("\n");
    }
    printf("\n");
}


//=======================IMPRIMIR CAMINHO=======================

void printPath(Node* meet1, Node* meet2) {

    Node* path1[MAX_NODES];
    Node* path2[MAX_NODES];

    int i = 0, j = 0;

    // Caminho do início até encontro
    while (meet1 != NULL) {
        path1[i++] = meet1;
        meet1 = meet1->pai;
    }

    // Caminho do encontro até objetivo
    while (meet2 != NULL) {
        path2[j++] = meet2;
        meet2 = meet2->pai;
    }

    printf("Caminho solução:\n\n");

    // Imprime início -> encontro
    for (int k = i - 1; k >= 0; k--) {
        printEstado(path1[k]->estado);
    }

    // Imprime encontro -> fim (sem repetir)
    for (int k = 1; k < j; k++) {
        printEstado(path2[k]->estado);
    }
}


//=======================BUSCA BIDIRECIONAL=======================
void bidirectionalSearch(Estado start, Estado goal) {

    // Inicializando filas
    Fila fila1, fila2;
    initFila(&fila1);
    initFila(&fila2);

    // Inicializando os no inicial (busca 1)
    Node* startNode = malloc(sizeof(Node));
    startNode->estado = start;
    startNode->pai = NULL;

    // Inicializando o nó final (busca 2)
    Node* goalNode = malloc(sizeof(Node));
    goalNode->estado = goal;
    goalNode->pai = NULL;

    // Inserimos os nós inicializados em suas respectivas filas
    push(&fila1, startNode);
    push(&fila2, goalNode);

    // Inicializamos o vetor contendo os nós visitados
    Node* visited1[MAX_NODES];
    Node* visited2[MAX_NODES];

    int size1 = 0, size2 = 0;// tamanho de cada vetor dos nós visitados

    // Armazenamos os nossos nós incial e final
    visited1[size1++] = startNode;
    visited2[size2++] = goalNode;

    while (!isVazio(&fila1) && !isVazio(&fila2)) {

        //----------EXPANSÃO DO INÍCIO----------
        Node* current1 = pop(&fila1);

        Estado neighbors[4];

        // Check de movimentos possíveis
        int n = getNeighbors(current1->estado, neighbors);

        for (int i = 0; i < n; i++) {

            // Verifica se o estado ainda não foi visitado
            if (!contains(neighbors[i], visited1, size1)) {

                // Nó auxiliar
                Node* newNode = malloc(sizeof(Node));
                newNode->estado = neighbors[i];
                newNode->pai = current1;

                // Armazenamento do estado nos nós visitados
                visited1[size1++] = newNode;
                push(&fila1, newNode);

                // Verifica interseção
                for (int j = 0; j < size2; j++) {
                    if (isEqual(neighbors[i], visited2[j]->estado)) {
                        // Se sim, imprime
                        printPath(newNode, visited2[j]);
                        return;
                    }
                }
            }
        }
  
        //----------EXPANSÃO DO FIM----------
        Node* current2 = pop(&fila2);

        n = getNeighbors(current2->estado, neighbors);

        for (int i = 0; i < n; i++) {

            if (!contains(neighbors[i], visited2, size2)) {

                Node* newNode = malloc(sizeof(Node));
                newNode->estado = neighbors[i];
                newNode->pai = current2;

                visited2[size2++] = newNode;
                push(&fila2, newNode);

                // Verifica interseção
                for (int j = 0; j < size1; j++) {
                    if (isEqual(neighbors[i], visited1[j]->estado)) {
                        printPath(visited1[j], newNode);
                        return;
                    }
                }
            }
        }
    }

    printf("Sem solução.\n");
}