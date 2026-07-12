// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "knowledge.h"

static int DI[4] = {-1, 1, 0, 0};
static int DJ[4] = {0, 0, -1, 1};

KB newKB(int h, int w){
    KB kb;
    kb.h = h; kb.w = w;
    kb.visited   = malloc(h*sizeof(bool*));
    kb.noBuraco  = malloc(h*sizeof(bool*));
    kb.noMonstro = malloc(h*sizeof(bool*));
    for (int i=0; i<h; i++){
        kb.visited[i]   = calloc(w, sizeof(bool));
        kb.noBuraco[i]  = calloc(w, sizeof(bool));
        kb.noMonstro[i] = calloc(w, sizeof(bool));
    }
    return kb;
}

void delKB(KB* kb){
    for (int i=0; i<kb->h; i++){
        free(kb->visited[i]);
        free(kb->noBuraco[i]);
        free(kb->noMonstro[i]);
    }
    free(kb->visited);
    free(kb->noBuraco);
    free(kb->noMonstro);
}

static bool inBounds(KB kb, int i, int j){
    return (i>=0 && i<kb.h && j>=0 && j<kb.w);
}

void updateKB(KB* kb, place* p){
/*
    Chamado assim que o agente chega em p (ou seja, p já foi observado
    diretamente). Aplica a regra de inferência às vizinhanças de p.
*/
    kb->visited[p->row][p->col] = true;
    for (int d=0; d<4; d++){
        int ni = p->row + DI[d];
        int nj = p->col + DJ[d];
        if (!inBounds(*kb, ni, nj)) continue;
        if (!p->S.vento)  kb->noBuraco[ni][nj]  = true;
        if (!p->S.cheiro) kb->noMonstro[ni][nj] = true;
    }
}

bool isFullySafe(KB kb, int i, int j){
    return kb.noBuraco[i][j] && kb.noMonstro[i][j];
}

bool isBuracoSafe(KB kb, int i, int j){
    return kb.noBuraco[i][j];
}

typedef struct { int r, c; } coord;

place* chooseTarget(KB kb, enviroment E, agent A, bool wantExit){
    int H = E.h, W = E.w;
    int start_r = A.onde->row, start_c = A.onde->col;
    int exit_r = H-1, exit_c = W-1;

    int**  dist   = malloc(H*sizeof(int*));
    coord** parent = malloc(H*sizeof(coord*));
    for (int i=0; i<H; i++){
        dist[i] = malloc(W*sizeof(int));
        parent[i] = malloc(W*sizeof(coord));
        for (int j=0; j<W; j++){ dist[i][j] = -1; parent[i][j] = (coord){-1,-1}; }
    }

    /*
        BFS restrito às células já visitadas E que não são buracos (o
        agente só pode "reatravessar" com segurança um local que já
        conhece e que não seja um buraco; monstros já mortos não
        oferecem mais risco).
    */
    coord* queue = malloc(H*W*sizeof(coord));
    int qh=0, qt=0;
    dist[start_r][start_c] = 0;
    queue[qt++] = (coord){start_r, start_c};
    while (qh < qt){
        coord cur = queue[qh++];
        for (int d=0; d<4; d++){
            int nr = cur.r + DI[d], nc = cur.c + DJ[d];
            if (!inBounds(kb, nr, nc)) continue;
            if (!kb.visited[nr][nc]) continue;
            if (E.grid[nr][nc].buraco) continue; // nunca reatravessar buraco conhecido
            if (dist[nr][nc] != -1) continue;
            dist[nr][nc] = dist[cur.r][cur.c] + 1;
            parent[nr][nc] = cur;
            queue[qt++] = (coord){nr, nc};
        }
    }

    // Se buscamos a saída e ela já foi visitada anteriormente (logo, já
    // sabemos que é segura), basta seguir o caminho conhecido até lá.
    if (wantExit && dist[exit_r][exit_c] != -1){
        place* result;
        if (exit_r == start_r && exit_c == start_c){
            result = &E.grid[exit_r][exit_c];
        } else {
            coord cur = {exit_r, exit_c};
            while (!(parent[cur.r][cur.c].r == start_r && parent[cur.r][cur.c].c == start_c)){
                cur = parent[cur.r][cur.c];
            }
            result = &E.grid[cur.r][cur.c];
        }
        for (int i=0; i<H; i++){ free(dist[i]); free(parent[i]); }
        free(dist); free(parent); free(queue);
        return result;
    }

    /*
        Reúne candidatos: células NÃO visitadas adjacentes a alguma célula
        alcançável pelo BFS acima. Classifica em 3 níveis de risco:
        tier 0: com certeza sem buraco e sem monstro
        tier 1: com certeza sem buraco; monstro é tolerável APENAS se o
                 agente ainda tiver a flecha (custo fixo de -10)
        tier 2: risco de morte potencial (-1000): ou o buraco é
                 desconhecido, ou há risco de monstro e a flecha já foi
                 usada — usada apenas se não houver alternativa
    */
    int bestTier = INT_MAX;
    long bestMetric = LONG_MAX;
    coord bestEntry = {-1,-1}, bestTarget = {-1,-1};
    bool foundExitCandidate = false;
    coord exitEntry = {-1,-1};

    for (int r=0; r<H; r++){
        for (int c=0; c<W; c++){
            if (dist[r][c] == -1) continue; // não alcançável ainda
            for (int d=0; d<4; d++){
                int nr = r + DI[d], nc = c + DJ[d];
                if (!inBounds(kb, nr, nc)) continue;
                if (kb.visited[nr][nc]) continue; // já visitado, não é fronteira

                bool buracoUnknown = !isBuracoSafe(kb, nr, nc);
                bool monstroUnknown = !kb.noMonstro[nr][nc];
                // sem flecha, um possível monstro é tão fatal quanto um
                // possível buraco (-1000 em ambos os casos)
                bool monstroFatal = monstroUnknown && !A.temFlecha;

                int tier;
                long metric;
                if (!buracoUnknown && !monstroUnknown) tier = 0;
                else if (!buracoUnknown && !monstroFatal) tier = 1; // monstro tolerável (tem flecha)
                else {
                    tier = 2;
                    // heurística de risco: vizinhos que sentiram vento (indício
                    // de buraco) e, se o monstro for fatal aqui, também cheiro
                    // (indício de monstro por perto)
                    int risk = 0;
                    for (int d2=0; d2<4; d2++){
                        int vr = nr + DI[d2], vc = nc + DJ[d2];
                        if (!inBounds(kb, vr, vc)) continue;
                        if (!kb.visited[vr][vc]) continue;
                        if (buracoUnknown && E.grid[vr][vc].S.vento) risk++;
                        if (monstroFatal && E.grid[vr][vc].S.cheiro) risk++;
                    }
                    metric = risk * 1000L + (dist[r][c] + 1);
                }
                if (tier != 2) metric = dist[r][c] + 1; // pathDist para tiers seguros

                // se buscando a saída, prioriza minimizar dist. Manhattan até a saída
                if (wantExit){
                    if (nr == exit_r && nc == exit_c){
                        foundExitCandidate = true;
                        exitEntry = (coord){r,c};
                    }
                    int manh = abs(nr-exit_r) + abs(nc-exit_c);
                    metric = manh * 1000L + (dist[r][c] + 1);
                }

                if (tier < bestTier || (tier == bestTier && metric < bestMetric)){
                    bestTier = tier;
                    bestMetric = metric;
                    bestEntry = (coord){r,c};
                    bestTarget = (coord){nr,nc};
                }
            }
        }
    }

    // Se está de posse do ouro e a saída já é um alvo de fronteira viável
    // (tier <=1), vá direto para ela — é a jogada vencedora.
    if (wantExit && foundExitCandidate){
        bool exitMonstroOk = kb.noMonstro[exit_r][exit_c] || A.temFlecha;
        bool exitSafeEnough = dist[exit_r][exit_c] != -1 ||
            (isBuracoSafe(kb, exit_r, exit_c) && exitMonstroOk);
        if (exitSafeEnough){
            bestEntry = exitEntry;
            bestTarget = (coord){exit_r, exit_c};
        }
    }

    place* result = NULL;
    if (bestTarget.r != -1){
        if (bestEntry.r == start_r && bestEntry.c == start_c){
            result = &E.grid[bestTarget.r][bestTarget.c];
        } else {
            coord cur = bestEntry;
            while (!(parent[cur.r][cur.c].r == start_r && parent[cur.r][cur.c].c == start_c)){
                cur = parent[cur.r][cur.c];
            }
            result = &E.grid[cur.r][cur.c];
        }
    }

    for (int i=0; i<H; i++){ free(dist[i]); free(parent[i]); }
    free(dist); free(parent); free(queue);
    return result;
}