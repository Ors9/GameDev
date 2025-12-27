#include "game_state.h"
#include "stdio.h"
#include "stdlib.h"

typedef struct GameState
{
    MainGameScreenState main_game_screen_state;

    SubStateLogin login_sub_state;
    SubStateGameplay gameplay_sub_state;
    SubStateExit exit_sub_state;

};

GameState *  InitGameState(){

    GameState * gS = malloc(sizeof(GameState));
    if(gS == NULL){
        printf("Failed to allocate memory for GameState.\n");
        exit(1);
    }

    gS->main_game_screen_state = STATE_LOGIN;
    gS->login_sub_state = SUB_LOGIN_WAITING;
    gS->gameplay_sub_state = SUB_GAMEPLAY_NONE;
    gS->exit_sub_state = SUB_EXIT_NONE;

    return gS;
}


GameState * UnloadGameState(GameState * gS){
    free(gS);
    return NULL;
}




