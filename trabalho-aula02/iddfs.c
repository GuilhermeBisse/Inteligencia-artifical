// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito
#include "iddfs.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

// Contadores globais que usamos para medir quantos nós foram expandidos durante a busca.
// g_nodes_expanded conta os nós da iteração atual, e g_total_nodes acumula o total de todas as iterações.
long g_nodes_expanded = 0;
long g_total_nodes = 0;

// Implementação de uma pilha simples que usamos internamente no IDDFS.
// Cada item guarda um nó da árvore e um ponteiro para o próximo item da pilha.

Stack* newStack(void) {
    Stack* s = malloc(sizeof(Stack));
    assert(s);
    s->top = NULL;
    s->size = 0;
    return s;
}

void pushStack(Stack* s, node nd) {
    StackItem* it = malloc(sizeof(StackItem));
    assert(it);
    it->nd = nd;
    it->next = s->top;
    s->top = it;
    s->size++;
}

node popStack(Stack* s) {
    if (!s->top) return NULL;
    StackItem* it = s->top;
    node nd = it->nd;
    s->top  = it->next;
    s->size--;
    free(it);
    return nd;
}

void freeStack(Stack* s) {
    while (s->top) popStack(s);
    free(s);
}

// Para evitar ciclos durante a busca, mantemos um conjunto com todos os estados que estão
// no caminho atual (do nó raiz até o nó sendo explorado). Antes de expandir um filho,
// verificamos se ele já aparece nesse caminho, evitando repetições e loops infinitos.

PathSet* newPathSet(void) {
    PathSet* ps = malloc(sizeof(PathSet));
    assert(ps);
    ps->top = NULL;
    return ps;
}

void pushPath(PathSet* ps, game* state) {
    PathItem* it = malloc(sizeof(PathItem));
    assert(it);
    it->state = newGame();
    copyGame(state, it->state);
    it->next = ps->top;
    ps->top = it;
}

void popPath(PathSet* ps) {
    if (!ps->top) return;
    PathItem* it = ps->top;
    ps->top = it->next;
    delGame(it->state);
    free(it);
}

bool inPath(PathSet* ps, game* state) {
    for (PathItem* it = ps->top; it != NULL; it = it->next) {
        if (equal(it->state, state))
            return true;
    }
    return false;
}

void freePathSet(PathSet* ps) {
    while (ps->top) popPath(ps);
    free(ps);
}

// Implementação do DLS (Busca em Profundidade com Limite). Dado um nó e um limite de profundidade,
// ele explora recursivamente os filhos até encontrar a solução ou esgotar o limite. O argumento ps
// guarda os estados do caminho atual para evitar ciclos, de forma semelhante ao que fizemos com
// a fila de explorados no A*. Usamos um estado auxiliar para testar cada movimento sem modificar
// o estado do nó atual, e desfazemos o movimento ao final de cada tentativa.
node DLS(node nd, int depth, PathSet* ps) {
    g_nodes_expanded++;

    if (endGame(nd->state)) {
        return nd;
    }

    if (depth == 0) {
        return NULL;
    }

    game* aux = newGame();
    copyGame(nd->state, aux);

    node result = NULL;

    for (int k = 1; k < 9 && result == NULL; k++) {
        if (moveGame(aux, k)) {
            if (!inPath(ps, aux)) {
                node child = childNode(nd, k);

                pushPath(ps, child->state);
                result = DLS(child, depth - 1, ps);
                popPath(ps);

                if (result == NULL) {
                    delNode(child);
                }
            }
            // Desfaz o movimento para testar a próxima peça
            moveGame(aux, k);
        }
    }

    delGame(aux);
    return result;
}

// Implementação do IDDFS (Busca com Aprofundamento Iterativo). A ideia é executar o DLS repetidamente,
// aumentando o limite de profundidade a cada iteração (de 0 até maxDepth). Isso garante que a primeira
// solução encontrada tem o menor número de passos possível, combinando a completude da busca em largura
// com o baixo uso de memória da busca em profundidade. O custo disso é que os nós dos níveis mais rasos
// são expandidos múltiplas vezes, mas na prática isso é aceitável.
node IDDFS(game* G, int maxDepth) {
    g_total_nodes = 0;

    for (int depth = 0; depth <= maxDepth; depth++) {
        g_nodes_expanded = 0;

        node root = childNode(NULL, 0);
        copyGame(G, root->state);

        PathSet* ps = newPathSet();
        pushPath(ps, root->state);

        node result = DLS(root, depth, ps);

        freePathSet(ps);
        g_total_nodes += g_nodes_expanded;

        if (result != NULL) {
            return result;
        }

        delNode(root);
    }

    return NULL;
}

// Função de entrada para uso no searchGame.c, seguindo o mesmo padrão das funções AStarSearch e IDAStarSearch.
// Reseta os contadores antes de cada chamada e usa MAX_DEPTH_IDDFS como limite de profundidade.
node IDDFSSearch(game* G) {
    g_nodes_expanded = 0;
    g_total_nodes = 0;
    return IDDFS(G, MAX_DEPTH_IDDFS);
}
