#include "game_state.h"
#include "stdio.h"
#include "stdlib.h"
#include "auth/auth_ui_manager.h"
#include "auth/db_auth_manager.h"
#include "choose_character.h"
#include <assets_manager.h>
#include <camera_manager.h>
#include <user_session.h>

static void HandleStateLogin(GameState *gs);

struct GameState
{
    UserSession *session;
    AssetManager *assets;
    MainGameScreenState main_game_screen_state;
    SubStateLogin login_sub_state;
    SubStateGameplay gameplay_sub_state;
    SubStateExit exit_sub_state;
    GameCamera *mainCamera;
    PGconn *dataBase;
};

GameState *InitGameState(AssetManager *asset, GameCamera *mainCamera)
{

    GameState *gs = malloc(sizeof(GameState));
    if (gs == NULL)
    {
        printf("Failed to allocate memory for GameState.\n");
        exit(1);
    }

    gs->mainCamera = mainCamera;
    gs->assets = asset;
    gs->main_game_screen_state = STATE_LOGIN;
    gs->login_sub_state = SUB_LOGIN_ENTERING_NAME;
    gs->gameplay_sub_state = SUB_GAMEPLAY_NONE;
    gs->exit_sub_state = SUB_EXIT_NONE;
    gs->dataBase = ConnectToDatabase();

    gs->session = InitUserSession("", -1, false , NULL);

    return gs;
}

MainGameScreenState GetMainGameState(GameState *gs)
{
    return gs->main_game_screen_state;
}

void UpdateGameState(GameState *gs, MainGameScreenState newState)
{
    gs->main_game_screen_state = newState;
}

GameCamera *GetMainCamera(GameState *gs)
{
    return gs->mainCamera;
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

UserSession *GetUserSession(GameState * gs){
    return gs->session;
}

AssetManager *getAssetManager(GameState *gs)
{
    return gs->assets;
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
    case SUB_LOGIN_CHOOSE_CHARACTER:
        ChoosePlayerScreen(1600, 900, gs);
        break;
    case SUB_LOGIN_CREATE_CHARACTER:
        CreateCharacterScreen(1600, 900, gs);
        break;
    case SUB_LOGIN_CONNECTING:
        break;
    }
}

PGconn *getDataBase(GameState *gs)
{
    return gs->dataBase;
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
        // To do Start game
        break;
    case STATE_EXIT:
        break;
    }
}

SubStateLogin getLoginState(GameState *gs)
{
    return gs->login_sub_state;
}

GameState *UnloadGameState(GameState *gs)
{

    if (gs == NULL)
    {
        return NULL;
    }

    // 1. סגירת מסד הנתונים
    if (gs->dataBase != NULL)
    {
        PQfinish(gs->dataBase);
        gs->dataBase = NULL;
        puts("Database closed.");
    }

    if (gs->session != NULL)
    {
        UnloadUserSession(gs->session);
        gs->session = NULL;
    }

    // 4. שחרור המבנה עצמו
    free(gs);
    gs = NULL;

    printf("GameState unloaded successfully.\n");

    return NULL;
}
