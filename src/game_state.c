#include "game_state.h"
#include "stdio.h"
#include "stdlib.h"
#include "ui_manager.h"

static void HandleStateLogin(GameState *gs);

struct GameState
{
    MainGameScreenState main_game_screen_state;

    SubStateLogin login_sub_state;
    SubStateGameplay gameplay_sub_state;
    SubStateExit exit_sub_state;
};

GameState *InitGameState()
{

    GameState *gs = malloc(sizeof(GameState));
    if (gs == NULL)
    {
        printf("Failed to allocate memory for GameState.\n");
        exit(1);
    }

    gs->main_game_screen_state = STATE_LOGIN;
    gs->login_sub_state = SUB_LOGIN_ENTERING_NAME;
    gs->gameplay_sub_state = SUB_GAMEPLAY_NONE;
    gs->exit_sub_state = SUB_EXIT_NONE;

    return gs;
}

void UpdateGameState(GameState *gs, MainGameScreenState newState)
{
    gs->main_game_screen_state = newState;
}

void UpdateLoginState(GameState *gs, SubStateLogin newState)
{
    gs->login_sub_state = newState;
}

void UpdateGameplayState(GameState *gs, SubStateGameplay newState)
{
    gs->gameplay_sub_state = newState;
}

void UpdateExitState(GameState *gs, SubStateExit newState)
{
    gs->exit_sub_state = newState;
}

static void HandleStateLogin(GameState *gs)
{
    SubStateLogin loginState = gs->login_sub_state;
    switch (loginState)
    {
    case SUB_LOGIN_NONE:
        break;
    case SUB_LOGIN_WAITING:
        break;
    case SUB_LOGIN_REGISTERING:
        RegisterScreen(1600, 900, gs);
        break;
    case SUB_LOGIN_ENTERING_NAME:
        EnterGameScreen(1600, 900, gs);
        break;
    case SUB_LOGIN_CONNECTING:
        break;
    }
}



void HandleCurrentScreenState(GameState *gs)
{
    MainGameScreenState mainState = gs->main_game_screen_state;
    switch (mainState)
    {
    case STATE_LOGIN:
        HandleStateLogin(gs);
        break;
    case STATE_GAMEPLAY:
        //To do Start game
        break;
    case STATE_EXIT:
        break;
    }
}

SubStateLogin  getLoginState(GameState *gs)
{
    return gs->login_sub_state;
}


GameState *UnloadGameState(GameState *gs)
{
    free(gs);
    return NULL;
}
