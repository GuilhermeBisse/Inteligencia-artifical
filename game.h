
#include <stdbool.h>

#ifndef GAME_H
#define GAME_H

typedef struct{
    int** grid;
} game;

game* newGame();
void delGame(game* G);
void initGame(game* G);
bool endGame(game* G);
void printGame(game* G);
void copyGame(game* G, game* target);
bool moveGame(game* G, int num);
bool equal(game* A, game* B);

#endif