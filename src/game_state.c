#include "game_state.h"
#include "stdio.h"
#include "stdlib.h"
#include "auth/auth_ui_manager.h"
#include "auth/db_auth_manager.h"
#include "login/choose_character.h"
#include "assets/assets_manager.h"
#include <camera_manager.h>
#include <user_session.h>
#include "character_session.h"

#include "word/map_manager.h"

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
    GameMap *currMap;
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
    gs->currMap = NULL;
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
    GameMap *map = GetMap(gs);
    float deltaTime = GetFrameTime();
    static int x;
    UpdatePlayer(p, deltaTime, gs);

    UpdateGameCamera(gCam, p, deltaTime);

    // 1. צובעים את השמיים לפני הכל (מחוץ ל-3D)
    ClearBackground(WHITE);

    BeginMode3D(GetRaylibCamera(gCam));

    // 3. מציירים את העולם
    AssetManager *assets = getAssetManager(gs);
    DrawGrid(200, 10.0f);
    DrawMap(map, assets);

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

GameMap *GetMap(GameState *gs)
{
    return gs->currMap;
}

void SetNewMap(GameState *gs, GameMap *map)
{
    gs->currMap = map;
}

// פונקציה שמחזירה את הגובה המדויק של הקרקע מתחת לשחקן
float GetTerrainHeight(Model terrain, Vector3 playerPos, Vector3 terrainOffset, float terrainScale)
{
    // 1. יצירת קרן שמתחילה גבוה מעל השחקן ויורה למטה
    Ray ray = {0};
    ray.position = (Vector3){playerPos.x, playerPos.y + 50.0f, playerPos.z};
    ray.direction = (Vector3){0.0f, -1.0f, 0.0f}; // כיוון למטה

    // 2. יצירת מטריצה שמייצגת את המיקום והגודל של המודל בעולם
    // זה חשוב כי ציינת שהזזת אותו ל-220 והגדלת ב-5.0
    Matrix transform = MatrixMultiply(
        MatrixScale(terrainScale, terrainScale, terrainScale),
        MatrixTranslate(terrainOffset.x, terrainOffset.y, terrainOffset.z));

    // 3. בדיקת התנגשות בין הקרן לבין ה-Mesh של המודל
    RayCollision hit = GetRayCollisionMesh(ray, terrain.meshes[0], transform);

    if (hit.hit)
    {
        return hit.point.y; // זה הגובה המדויק של הסלע בנקודה הזו
    }

    return terrainOffset.y; // ברירת מחדל אם אין פגיעה
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

    if (gs->currMap != NULL)
    {
        UnloadGameMap(gs->currMap);
        gs->currMap = NULL;
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
