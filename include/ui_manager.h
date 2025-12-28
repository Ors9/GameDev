
#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "game_state.h"

typedef struct AuthUiState AuthUiState;


void EnterGameScreen(int screenWidth, int screenHeight , GameState * gameState);
void RegisterScreen(int screenWidth, int screenHeight , GameState *gameState);



#endif // UI_MANAGER_H