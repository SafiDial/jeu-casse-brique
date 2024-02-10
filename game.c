#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include <SDL2/SDL_ttf.h>

// Déclaration de la police
TTF_Font *font;
TTF_Font *gameOverFont;  // Ajout de la déclaration pour la police de "Game Over"

// Déclarations des variables globales
int fontSize = 18; // Taille de la police
void renderGameOver();// Déclaration de la fonction pour afficher "Game Over"
void initializeBall(); 
int ballX, ballY, ballSpeedX, ballSpeedY;
int paddleX, paddleY;
int lives = 4; //  le nombre de vies
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

// Définition des paramètres pour les briques
int numBricksX = 3;        // Nombre de briques dans la direction horizontale
int numBricksY = 3;        // Nombre de briques dans la direction verticale
int brickWidth = 60;       // Largeur d'une brique
int brickHeight = 20;      // Hauteur d'une brique
int brickSpacing = 10;     // Espacement entre les briques
int brickTopPadding = 30;  // Espace en haut des briques

// Tableau pour suivre l'état des briques (true pour actif, false pour désactivé)
bool bricks[3][3];

// Fonction d'initialisation du jeu
int initializeGame() {
    // Initialiser SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Créer la fenêtre
    window = SDL_CreateWindow("Casse-Briques", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_GL_SetSwapInterval(1);

    // Créer le rendu
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Initialiser SDL_ttf
    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return -1;
    }

    // Charger la police
    font = TTF_OpenFont("/home/safi/casse_briques.c/Police.ttf", fontSize);
    if (font == NULL) {
        fprintf(stderr, "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return -1;
    }

    // Appeler la fonction d'initialisation du jeu
    initialize();

    return 0;
}

// Fonction de nettoyage du jeu à la fin
void cleanupGame() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);  // Fermer la police avant de quitter
    TTF_Quit();
    SDL_Quit();
}
// Fonction d'initialisation des variables du jeu
void initialize() {
    // Initialiser la position de la balle, de la raquette, etc.
    ballX = SCREEN_WIDTH / 2;
    ballY = SCREEN_HEIGHT / 2;
    ballSpeedX = 5;   // Vitesse horizontale
    ballSpeedY = -5;  // Vitesse verticale (négative pour aller vers le haut)

    paddleX = (SCREEN_WIDTH - PADDLE_WIDTH) / 2;
    paddleY = SCREEN_HEIGHT - PADDLE_HEIGHT - 10;

    // Initialiser l'état des briques à true (actif)
    for (int row = 0; row < numBricksY; row++) {
        for (int col = 0; col < numBricksX; col++) {
            bricks[row][col] = true;
        }
    }
    
    lives = 3; // Initialiser le nombre de vies
}

// Fonction pour afficher le nombre de vies
void renderLives() {
    // Convertir le nombre de vies en chaîne de caractères
    char livesText[20];
    snprintf(livesText, sizeof(livesText), "Lives: %d", lives);

    // Créer une surface de texte à partir de la chaîne de caractères
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, livesText, (SDL_Color){255, 255, 255, 255});
    if (textSurface == NULL) {
        fprintf(stderr, "Failed to render lives text! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    // Créer une texture à partir de la surface de texte
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        fprintf(stderr, "Failed to create texture from lives text! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    // Dessiner la texture de texte à l'écran
    SDL_Rect textRect = {10, 10, textSurface->w, textSurface->h}; // Position du texte en haut à gauche
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Libérer la surface de texte et la texture
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}



// Fonction de mise à jour du jeu
void update() {
    ballX += ballSpeedX;
    ballY += ballSpeedY;

    if (ballX - BALL_RADIUS < 0 || ballX + BALL_RADIUS > SCREEN_WIDTH) {
        ballSpeedX = -ballSpeedX;
    }

    if (ballY - BALL_RADIUS < 0) {
        ballSpeedY = -ballSpeedY;
    }

    if (ballY + BALL_RADIUS > paddleY && ballX > paddleX && ballX < paddleX + PADDLE_WIDTH) {
        ballSpeedY = -ballSpeedY;
    }

    // Vérifier la collision avec les briques
    for (int row = 0; row < numBricksY; row++) {
        for (int col = 0; col < numBricksX; col++) {
            if (bricks[row][col]) {
                int brickX = col * (brickWidth + brickSpacing) + (SCREEN_WIDTH - (numBricksX * (brickWidth + brickSpacing))) / 2;
                int brickY = row * (brickHeight + brickSpacing) + brickTopPadding;

                if (ballX + BALL_RADIUS > brickX && ballX - BALL_RADIUS < brickX + brickWidth &&
                    ballY + BALL_RADIUS > brickY && ballY - BALL_RADIUS < brickY + brickHeight) {
                    // La balle touche la brique, désactivez la brique
                    bricks[row][col] = false;

                    // Inverser la direction de la balle en fonction de la collision
                    ballSpeedY = -ballSpeedY;
                }
            }
        }
    }

    // Si la balle touche le bas de l'écran
   // Si la balle touche le bas de l'écran
if (ballY + BALL_RADIUS > SCREEN_HEIGHT) {
    // Si la balle n'a pas touché la barre, réinitialiser la position de la balle sur la barre
    if (!(ballX > paddleX && ballX < paddleX + PADDLE_WIDTH)) {
        initializeBall();
    }

    int totalBricks = numBricksX * numBricksY;
    int bricksHit = 0;

    for (int row = 0; row < numBricksY; row++) {
        for (int col = 0; col < numBricksX; col++) {
            if (!bricks[row][col]) {
                bricksHit++;
            }
        }
    }

    if (lives <= 0) {
    printf("Game Over: Lives exhausted\n");
        // Si le nombre de vies est épuisé, afficher "Game Over" et quitter le jeu
        renderGameOver();
        SDL_Delay(2000);  // Attendre 2 secondes pour afficher "Game Over"
        cleanupGame();    // Nettoyer et quitter le jeu
        exit(0);
    } else if (bricksHit == totalBricks) {
    printf("Game Over: Lives exhausted\n");
        // Si toutes les briques sont désactivées, afficher "Game Over" et quitter le jeu
        renderGameOver();
        SDL_Delay(2000);  // Attendre 2 secondes pour afficher "Game Over"
        cleanupGame();    // Nettoyer et quitter le jeu
        exit(0);
    } else {
        // Si le nombre de vies n'est pas épuisé et des briques restent, décrémenter le nombre de vies
        lives--;

        // Réinitialiser la position de la balle si des briques restent
        initializeBall();
    }
}
}







// Fonction de gestion des entrées utilisateur
void handleInput() {
    SDL_Event e;
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            exit(0);
        }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                exit(0);
            }
        }
    }

    if (state[SDL_SCANCODE_LEFT] && paddleX > 0) {
        paddleX -= 5;
    }
    if (state[SDL_SCANCODE_RIGHT] && paddleX < SCREEN_WIDTH - PADDLE_WIDTH) {
        paddleX += 5;
    }
}


// Fonction de rendu graphique
void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &(SDL_Rect){paddleX, paddleY, PADDLE_WIDTH, PADDLE_HEIGHT});
    SDL_RenderFillRect(renderer, &(SDL_Rect){paddleX, paddleY, PADDLE_WIDTH, PADDLE_HEIGHT});

    // Dessiner la balle (cercle)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < BALL_RADIUS * 2; i++) {
        for (int j = 0; j < BALL_RADIUS * 2; j++) {
            int dx = BALL_RADIUS - i; // calculer les distances par rapport au centre
            int dy = BALL_RADIUS - j;
            if ((dx * dx + dy * dy) <= (BALL_RADIUS * BALL_RADIUS)) {
                SDL_RenderDrawPoint(renderer, ballX - BALL_RADIUS + i, ballY - BALL_RADIUS + j);
            }
        }
    }

    // Dessiner le nombre de vies
    renderLives();

    // Dessiner les briques
    for (int row = 0; row < numBricksY; row++) {
        for (int col = 0; col < numBricksX; col++) {
            if (bricks[row][col]) {
                SDL_RenderDrawRect(renderer, &(SDL_Rect){col * (brickWidth + brickSpacing) + (SCREEN_WIDTH - (numBricksX * (brickWidth + brickSpacing))) / 2, row * (brickHeight + brickSpacing) + brickTopPadding, brickWidth, brickHeight});
                SDL_RenderFillRect(renderer, &(SDL_Rect){col * (brickWidth + brickSpacing) + (SCREEN_WIDTH - (numBricksX * (brickWidth + brickSpacing))) / 2, row * (brickHeight + brickSpacing) + brickTopPadding, brickWidth, brickHeight});
            }
        }
    }

    SDL_RenderPresent(renderer);
}


// Fonction pour réinitialiser la position de la balle
void initializeBall() {
    ballX = SCREEN_WIDTH / 2;
    ballY = SCREEN_HEIGHT / 2;
    ballSpeedX = 5;
    ballSpeedY = -5;
}

// Fonction pour afficher "Game Over"
void renderGameOver() {
    // Effacer l'écran
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Utiliser SDL_ttf pour afficher "Game Over" à l'écran
    SDL_Color color = {255, 255, 255, 255};
    TTF_Font *gameOverFont = TTF_OpenFont("/home/safi/casse_briques.c/Police.ttf", 40);  // la taille de la police

    if (gameOverFont == NULL) {
        fprintf(stderr, "Failed to load font for Game Over! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    char gameOverText[] = "Game Over";
    SDL_Surface *textSurface = TTF_RenderText_Solid(gameOverFont, gameOverText, color);
    if (textSurface == NULL) {
        fprintf(stderr, "Failed to render Game Over text! SDL_ttf Error: %s\n", TTF_GetError());
        TTF_CloseFont(gameOverFont);
        return;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        fprintf(stderr, "Failed to create texture from Game Over text! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(gameOverFont);
        return;
    }

    // Centrer le texte à l'écran
    SDL_Rect textRect = {(SCREEN_WIDTH - textSurface->w) / 2, (SCREEN_HEIGHT - textSurface->h) / 2, textSurface->w, textSurface->h};

    // Dessiner le texte "Game Over" à l'écran
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_RenderPresent(renderer);

    // Attendre quelques secondes
    SDL_Delay(2000);

    // Libérer les ressources
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(gameOverFont);
}
