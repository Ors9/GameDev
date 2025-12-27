#include "game_loop.h"
#include "player.h"
#include "enemy.h"
#include "camera_manager.h"
#include "game_state.h" // שינוי למירכאות


void StartGame()
{
    const int screenWidth = 1600; 
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Summoner Game");

    // אתחול ישויות
    Player *player = InitPlayer();
    Enemy *enemy = InitEnemy();


    GameState gameState;
    InitGameState(&gameState);

    GameCamera *gameCamera = InitGameCamera();
  

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // --- 1. Update (לוגיקה) ---
        UpdateEnemy(enemy, player, deltaTime);
        UpdatePlayer(player, deltaTime); 
        UpdateGameCamera(gameCamera, player, deltaTime);

        // --- 2. Draw (ציור) ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(GetRaylibCamera(gameCamera));
        DrawGrid(1000, 2.0f);
        DrawPlayer(player); 
        DrawEnemy(enemy);     
        EndMode3D();

        DrawFPS(10, 10);
        EndDrawing();
    }

    // שחרור זיכרון מסודר
    UnloadPlayer(player);
    UnloadEnemy(enemy);
    CloseWindow();
}