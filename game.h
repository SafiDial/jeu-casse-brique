
#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <SDL2/SDL.h>

// Définition des constantes
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 20
#define BALL_RADIUS 10
 void renderGameOver();
// Déclarations des fonctions
int initializeGame();

void cleanupGame();
void initialize();
void render();
void update();
void handleInput();
void renderLives();


// Déclaration de la variable globale
extern int lives;

#endif  // GAME_H

