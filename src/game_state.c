#include "game_state.h"



void InitGameState(GameState *gameState){

    gameState->main_game_screen_state = STATE_LOGIN;
    gameState->login_sub_state = SUB_LOGIN_WAITING;
    gameState->gameplay_sub_state = SUB_GAMEPLAY_NONE;
    gameState->exit_sub_state = SUB_EXIT_NONE;
}


