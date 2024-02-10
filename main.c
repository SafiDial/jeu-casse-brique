#include <SDL2/SDL.h>
#include "game.h"

int main() {
    if (initializeGame() != 0) {
        fprintf(stderr, "Failed to initialize the game!\n");
        return -1;
    }

    while (lives > 0) {  // Boucle principale du jeu
        handleInput();
        update();
        render();
        SDL_Delay(16);  // Ajoutez une petite pause pour contrôler la fréquence
    }

    cleanupGame();  // Nettoyez et quittez le jeu à la fin

    return 0;
}

