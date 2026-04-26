// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito
#include "bfstree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
//#include <windows.h> função sleep no windows

fila newFila(){
    fila f = malloc(sizeof(Fila));
    assert(f);
    f->first = NULL;
    f->last = NULL;
    return f;
}

void insert(fila f, node newNode){
/*
    Insere newNode na última posição de f
*/
    item I = malloc(sizeof(struct Item));
    assert(I);
    I->nd = newNode;
    I->next = NULL;
    if (f->last==NULL){
        assert(f);
        f->first = I;
        f->last = I;
    }
    else{
        f->last->next = I;
        f->last = I;
    }
}

node pop(fila f){
/*
    Retorna o primeiro nó da fila, removendo-o da mesma.
*/
    item I = f->first;
    node nd = I->nd;
    f->first = I->next;
    if (f->first==NULL){
        f->last = NULL;
    }
    free(I);
    return nd;
}


// Implementação que fizemos de uma função adicional para remover um item específico da fila, que utilizamos para remover um nó da fronteira ao ser explorado.
void removerNo(fila f, item alvo) {
    if (f == NULL || f->first == NULL || alvo == NULL) return;

    item ant = NULL;
    item atual = f->first;

    while (atual != NULL && atual != alvo) {
        ant = atual;
        atual = atual->next;
    }

    if (atual == NULL) return;

    if (ant == NULL) {
        f->first = atual->next;
    } else {
        ant->next = atual->next;
    }

    if (atual == f->last) {
        f->last = ant;
    }

    free(atual);
}

// Modificação na função isIn para verificar se um nó está presente na fila, comparando os estados dos nós utilizando a função equal. Essa função é utilizada para verificar se um nó filho gerado já foi explorado ou está presente na fronteira, evitando assim loops e inserções desnecessárias na fronteira.
bool isIn(fila f, node nd){

    for (item aux=f->first; aux!=NULL; aux=aux->next){
        if (equal(aux->nd->state,nd->state))
            return true;
    }
    return false;
}

int lenFila(fila f){
/*
    Número de elementos na fila. Foi implementado para rodar alguns testes.
*/
    int k = 0;
    for (item aux=f->first; aux!=NULL; aux=aux->next){
        k++;
    }
    return k;
}

node BFS(game* G){
/*
    Breadth-First Search (Busca em largura).
*/
    //Inicializa primeiro nó com a configuração fornecida do jogo.
    node nd = childNode(NULL,0);
    copyGame(G,nd->state);
    if (endGame(G)){
        return nd;
    }
    printGame(G); printf("\n");

    //Inicializa fronteira e conjunto de nós já explorados
    fila frontier = newFila();
    insert(frontier,nd);
    fila explored = newFila();
    insert(explored,nd);
    
    while(frontier->first!=NULL){
        //Armazena último nó em frontier
        node parent = pop(frontier);
        //Variável auxiliar
        game* aux = newGame();
        copyGame(parent->state,aux);
        //Cria nós filhos para cada ação possível
        for (int k=1; k<9; k++){
            //Checa se movimento é válido
            if (moveGame(aux,k)){
                node child = childNode(parent,k);
                if (!isIn(explored,child)){
                    //Checa se nó filho resolve o problema
                    if (endGame(child->state)){
                        return child;
                    }
                    //Caso contrário, insere nó filho em frontier
                    insert(frontier,child);
                    //Insere em explored
                    insert(explored,child);
                    //Impressão na tela
                    //sleep(1);
                    //printGame(child->state); printf("\n");
                }
                else{ //para poupar memória
                    delNode(child);
                }
                //Retorna para estado anterior ao movimento
                moveGame(aux,k);
            }
        }
        //Gerenciamento adequado de memória. Não é necessário, mas eu fiz
        delGame(aux);
    }
    
    return NULL;
}