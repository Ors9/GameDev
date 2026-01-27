#include "game_loop.h"
#include "characters.h"
#include "enemy.h"
#include "camera_manager.h"
#include "game_state.h" // שינוי למירכאות
#include "auth/auth_ui_manager.h"
#include "auth/db_auth_manager.h"
#include <assets_manager.h>

static void UnloadGamePointers(Player *player, Enemy *enemy, GameCamera *gCam, GameState *gameState, AssetManager *assets)
{
    UnloadPlayer(player);
    UnloadEnemy(enemy);
    UnloadGameCamera(gCam);
    UnloadGameState(gameState);  
    UnloadAssetsManager(assets); 
    printf("Done memory clearing.\n");
}

void StartGame()
{
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Summoner Game");
    AssetManager *assets = InitAssetManager();

    // אתחול ישויות
    //Player *player = InitPlayer(MUTANT_CHAR, assets); // MUTANT_CHAR MONSTER_CHAR
    //Enemy *enemy = InitEnemy();

    GameCamera *gCam = InitGameCamera();
    GameState *gameState = InitGameState(assets, gCam);
    // Model testModel = LoadModel("assets/model/Enivorment/floorGLB.glb"); // TESTTTTTTT

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        //HandleCurrentScreenState(gameState);

        // --- 1. Update (לוגיקה) ---
       // UpdateEnemy(enemy, player, deltaTime);
        //UpdatePlayer(player, deltaTime, gameState);
        //UpdateGameCamera(gCam, player, deltaTime);

        // --- 2. Draw (ציור) ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(GetRaylibCamera(gCam));
        DrawGrid(1000, 2.0f);
        //DrawPlayer(player);
        //DrawEnemy(enemy); // TESTTTTTTT

        // DrawModel(testModel, (Vector3)GetPlayerPosition(player), 10.0f, WHITE); // TESTTTTTTT

        //HandleCurrentScreenState(gameState);

        EndMode3D();

        HandleCurrentScreenState(gameState);

        DrawFPS(10, 10);
        EndDrawing();
    }

    // שחרור זיכרון מסודר
    //UnloadGamePointers(player, enemy, gCam, gameState, assets);

    CloseWindow();
}