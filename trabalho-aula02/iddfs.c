#include "iddfs.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

/* Contadores globais */

long g_nodes_expanded = 0;
long g_total_nodes = 0;

/* Pilha simples */

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
    s->top = it->next;
    s->size--;
    free(it);
    return nd;
}

void freeStack(Stack* s) {
    while (s->top) popStack(s);
    free(s);
}

/* Conjunto de estados no caminho atual (evita ciclos)*/

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

/* DLS — Busca em profundidade com limite */

/*DLS(nd, depth, ps)
 *
 * nd : nó atual
 * depth : profundidade restante permitida
 * ps : conjunto de estados no caminho atual (evita ciclos)
 *
 * Retorna o nó solução ou NULL.
 */
node DLS(node nd, int depth, PathSet* ps) {
    g_nodes_expanded++;

    /* Verificação da meta */
    if (endGame(nd->state)) {
        return nd;
    }

    /* Corte por profundidade */
    if (depth == 0) {
        return NULL;
    }

    /* Estado auxiliar para testar movimentos sem alterar nd->state */
    game* aux = newGame();
    copyGame(nd->state, aux);

    node result = NULL;

    /* Tenta cada ação possível: mover peça k (k = 1..8) */
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
            /* Restaura estado antes do próximo movimento */
            moveGame(aux, k);
        }
    }
    delGame(aux);
    return result;
}

/* IDDFS — Aprofundamento iterativo */

/*
 * IDDFS(G, maxDepth)
 *
 * Para d = 0, 1, 2, ..., maxDepth:
 *   Cria nó raiz com estado G.
 *   Inicializa PathSet com estado raiz.
 *   Chama DLS(raiz, d, ps).
 *   Se encontrar solução, retorna o nó.
 *
 * Retorna NULL se não encontrar dentro de maxDepth.
 * Acumula a contagem total de nós em g_total_nodes.
 */
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

node IDDFSSearch(game* G) {
    g_nodes_expanded = 0;
    g_total_nodes    = 0;
    return IDDFS(G, MAX_DEPTH_IDDFS);
}
