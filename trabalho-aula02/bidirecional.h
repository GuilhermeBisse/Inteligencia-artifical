#ifndef BIDIRECTIONAL_SEARCH_H
#define BIDIRECTIONAL_SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_NODES 10000

// Representa o tabuleiro 3x3 como um array de 9 posições
typedef struct {
    int tabuleiro[9];
} Estado;

// Nó da árvore de busca contém o estado e o ponteiro para o pai 
typedef struct Node {
    Estado estado;
    struct Node* pai;
} Node;

// Estrutura de Fila para a busca em largura
typedef struct {
    Node* items[MAX_NODES];
    int frente, tras;
} Fila;

// Protótipos das Funções

void initFila(Fila* fila);
bool isVazio(Fila* fila);
void push(Fila* fila, Node* value);
Node* pop(Fila* fila);

bool isEqual(Estado a, Estado b);
int getNeighbors(Estado s, Estado neighbors[]);
bool contains(Estado s, Node* visited[], int size);

Node* findIntersection(Node* visited1[], int size1, Node* visited2[], int size2);

void printEstado(Estado s);
void printPath(Node* meet1, Node* meet2);

// Função principaç
void bidirectionalSearch(Estado start, Estado goal);

#endif // BIDIRECTIONAL_SEARCH_H