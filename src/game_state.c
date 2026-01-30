#include "game_state.h"
#include "stdio.h"
#include "stdlib.h"
#include "auth/auth_ui_manager.h"
#include "auth/db_auth_manager.h"
#include "choose_character.h"
#include <assets_manager.h>
#include <camera_manager.h>
#include <user_session.h>
#include "character_session.h"
#include "rlgl.h"

static void HandleStateGameplay(GameState *gs);
static void HandleStateLogin(GameState *gs);
static void GamePlay(GameState *gs);

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

    gs->session = InitUserSession("", -1, false);

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

UserSession *GetUserSession(GameState *gs)
{
    return gs->session;
}

AssetManager *getAssetManager(GameState *gs)
{
    return gs->assets;
}

static void HandleStateGameplay(GameState *gs)
{
    SubStateGameplay gamplayState = gs->gameplay_sub_state;

    switch (gamplayState)
    {
    case SUB_GAMEPLAY_NONE:
        break;
    case SUB_GAMEPLAY_PAUSED:
        break;
    case SUB_GAMEPLAY_PLAYING:
        GamePlay(gs);
        // Playing game function..
        break;
    case SUB_GAMEPLAY_WAITING:
        break;
    }
}

static void GamePlay(GameState *gs)
{
    UserSession *us = gs->session;
    CharacterSession *cs = GetCharacterSession(us);
    GameCamera *gCam = GetMainCamera(gs);
    Player *p = GetPlayer(cs);
    float deltaTime = GetFrameTime();

    UpdatePlayer(p, deltaTime, gs);
    UpdateGameCamera(gCam, p, deltaTime);

    // 1. צובעים את השמיים לפני הכל (מחוץ ל-3D)
    ClearBackground((Color){135, 206, 235, 255});

    BeginMode3D(GetRaylibCamera(gCam));

    // 3. מציירים את העולם
    AssetManager *assets = getAssetManager(gs);
    DrawFloor(assets);
    DrawGrid(200, 10.0f);
    DrawPlayer(p);

    EndMode3D();

    DrawEntityLabel(gs);
}

void DrawWorldAxes(float length)
{
    // ציר X - אדום
    DrawLine3D((Vector3){0, 0, 0}, (Vector3){length, 0, 0}, RED);
    DrawCylinderEx((Vector3){length, 0, 0}, (Vector3){length + 2.0f, 0, 0}, 0.5f, 0.0f, 10, RED);

    // ציר Y - ירוק (למעלה)
    DrawLine3D((Vector3){0, 0, 0}, (Vector3){0, length, 0}, GREEN);
    DrawCylinderEx((Vector3){0, length, 0}, (Vector3){0, length + 2.0f, 0}, 0.5f, 0.0f, 10, GREEN);

    // ציר Z - כחול
    DrawLine3D((Vector3){0, 0, 0}, (Vector3){0, 0, length}, BLUE);
    DrawCylinderEx((Vector3){0, 0, length}, (Vector3){0, 0, length + 2.0f}, 0.5f, 0.0f, 10, BLUE);
}

void DrawFloor(AssetManager *asset)
{
    if (IsEnvResourceReady(asset, ENIV_WORD_TERRIAN))
    {
        Model terrain = GetEnvModelByType(asset, ENIV_WORD_TERRIAN);

        // 1. ציור חצים ענקיים (באורך 50 יחידות) כדי לדעת איפה אנחנו
        DrawWorldAxes(500.0f);

        // 3. הגנה מפני מודלים "חד-צדדיים"
        rlDisableBackfaceCulling();

        // 5. ציור המודל הרגיל

        DrawModel(terrain, (Vector3){0, 220, 0}, 5.0f, WHITE);

        rlEnableBackfaceCulling();
    }
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
        HandleStateGameplay(gs);
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
