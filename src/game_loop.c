#include "game_loop.h"
#include "player.h"
#include "enemy.h"
#include "camera_manager.h"
#include "game_state.h" // שינוי למירכאות
#include "ui_manager.h"

static void UnloadGamePointers(Player *player, Enemy *enemy, GameCamera *gCam, GameState *gameState)
{
    UnloadPlayer(player);
    UnloadEnemy(enemy);
    UnloadGameCamera(gCam);
    UnloadGameState(gameState);
}

void StartGame()
{
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Summoner Game");

    // אתחול ישויות
    Player *player = InitPlayer();
    Enemy *enemy = InitEnemy();

    GameState *gameState = InitGameState();
    GameCamera *gCam = InitGameCamera();

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        HandleCurrentScreenState(gameState);

        // --- 1. Update (לוגיקה) ---
        UpdateEnemy(enemy, player, deltaTime);
        UpdatePlayer(player, deltaTime);
        UpdateGameCamera(gCam, player, deltaTime);

        // --- 2. Draw (ציור) ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(GetRaylibCamera(gCam));
        DrawGrid(1000, 2.0f);
        DrawPlayer(player);
        DrawEnemy(enemy);

        HandleCurrentScreenState(gameState);

        EndMode3D();

        HandleCurrentScreenState(gameState);

        DrawFPS(10, 10);
        EndDrawing();
    }

    // שחרור זיכרון מסודר
    UnloadGamePointers(player, enemy, gCam, gameState);

    CloseWindow();
}