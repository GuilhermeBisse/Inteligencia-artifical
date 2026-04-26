// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito
#include "astar.h"

// Ponteiro para função heuristica, definido em searchGame.c
int (*heuristica)(game*);

// Implementação de uma função auxiliar recursiva para o algoritmo IDA* (Busca A* com aprofundamento iterativo). Ela é chamada pela função IDAStarSearch, e é responsável por explorar a árvore de busca até um certo limite, retornando o custo mínimo encontrado que ultrapassa esse limite, ou a solução caso ela seja encontrada. Por ser uma função recursiva, não precisamos armazenar os nós filhos em uma fila, como no A* tradicional. Esse algoritmo é geralmente mais eficiente em termos de memória. Contudo, a depender da heurística utilizada, ou da configuração inicial do jogo, ele pode demorar muito mais, e até dar segmentation fault se não for implementado corretamente (tivemos alguns erros com isso no início). Definimos um limite para o custo, e a cada iteração do IDA* esse limite é atualizado para o menor custo encontrado que ultrapassa o limite anterior. Assim, a função recursiva explora a árvore de busca, e retorna a solução caso ela seja encontrada, ou o menor custo encontrado que ultrapassa o limite caso contrário. O argumento pecaAnterior é utilizado para evitar que a função explore movimentos que desfazem o movimento anterior, o que pode levar a loops infinitos.
int func_recursiva_auxiliar(node n, int g, int limite, node* solucao, int pecaAnterior) {
    int f = g + heuristica(n->state);
    if (f > limite) return f;

    if (endGame(n->state)) {
        *solucao = n;
        return -1;
    }

    int min = 1000000;

    for (int k = 1; k < 9; k++) {
        if (k == pecaAnterior) continue;

        game* copia = newGame();
        copyGame(n->state, copia);

        if (moveGame(copia, k)) {
            node filho = childNode(n, k);
            copyGame(copia, filho->state);

            int temp = func_recursiva_auxiliar(filho, g + 1, limite, solucao, k);

            if (temp == -1) {
                delGame(copia);
                return -1;
            }

            if (temp < min) min = temp;

            delNode(filho);
        }

        delGame(copia);
    }

    return min;
}

// Algoritmo A* tradicional que implementamos usando duas filas - fronteira para guardar os nós a serem explorados, e explorados para guardar nós já explorados e evitar loops. A cada iteração, o nó com menor custo f = g + h é retirado da fronteira, e seus filhos são gerados. Se um filho é solução, ele é retornado. Caso contrário, ele é inserido na fronteira caso ainda não tenha sido explorado, ou seja, caso não esteja nem na fila de explorados, nem na fila de fronteira.
node AStarSearch(game* G){
    node raiz = childNode(NULL, 0);
    copyGame(G, raiz->state);

    fila fronteira = newFila();
    fila explorados = newFila();
    insert (fronteira, raiz);

    while(fronteira->first != NULL){
        item melhorItem = fronteira->first;
        for (item aux = fronteira->first; aux != NULL; aux = aux->next){
            int f_aux = aux->nd->pathCost + heuristica(aux->nd->state);
            int f_melhor = melhorItem->nd->pathCost + heuristica(melhorItem->nd->state);
            if (f_aux < f_melhor) melhorItem = aux;
        }

        node pai = melhorItem->nd;
        removerNo(fronteira, melhorItem);
        if(endGame(pai->state)) return pai;
        insert(explorados, pai);

        for (int k=1; k<9; k++){
            game* tempGame = newGame();
            copyGame(pai->state, tempGame);
            if (moveGame(tempGame, k)){
                node filho = childNode(pai, k);
                copyGame(tempGame, filho->state);
                if(!isIn(explorados, filho) && !isIn(fronteira, filho)){
                    insert(fronteira, filho);
                }
                else{
                    delNode(filho);
                }
            }
            delGame(tempGame);
        }
    }
    return NULL;
}

// Aqui a implementação do IDA* (Busca A* com aprofundamento iterativo), que é uma variação do A* tradicional que utiliza menos memória e possui menor complexidade. Essa função faz uso da função recursiva auxiliar que já explicamos acima. Aqui, o limite é inicialmente definido como a heurística do nó raiz, e a cada iteração do IDA* esse limite é atualizado para o menor custo encontrado que ultrapassa o limite anterior. Assim, a função recursiva explora a árvore de busca, e retorna a solução caso ela seja encontrada, ou o menor custo encontrado que ultrapassa o limite caso contrário. O argumento pecaAnterior é utilizado para evitar que a função explore movimentos que desfazem o movimento anterior, o que pode levar a loops infinitos.
node IDAStarSearch(game* G) {
    node raiz = childNode(NULL, 0);
    copyGame(G, raiz->state);

    int limite = heuristica(raiz->state);

    while (1) {
        node solucao = NULL;

        int temp = func_recursiva_auxiliar(raiz, 0, limite, &solucao, -1);

        if (temp == -1) return solucao;

        limite = temp;
    }
}