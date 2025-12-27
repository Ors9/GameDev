#include "game_loop.h"
#include "player.h"
#include "enemy.h"

static void InitCamera(Camera3D *camera);

// פונקציית עזר למצלמה (אפשר להשאיר כאן או להוציא לקובץ camera.h בעתיד)
static void InitCamera(Camera3D *camera)
{
    camera->fovy = 45.0f;
    camera->up = (Vector3){0.0f, 1.0f, 0.0f};
    camera->projection = CAMERA_PERSPECTIVE;
}

void StartGame()
{
    const int screenWidth = 2000;
    const int screenHeight = 1200;

    InitWindow(screenWidth, screenHeight, "Summoner Game");

    GameState gameState;
    InitGameState(&gameState);

    Player *player = InitPlayer();
    Enemy enemy;
    InitEnemy(&enemy);

    Camera3D camera = {0};
    InitCamera(&camera);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // --- 1. Update (לוגיקה וחישובים) ---
        float deltaTime = GetFrameTime();

        // עדכון אויב)
        UpdateEnemy(&enemy, player, deltaTime);

        UpdatePlayer(player, deltaTime);

 

        // עדכון מצלמה שעוקבת אחרי השחקן
        Vector3 playerPos = GetPlayerPosition(player);
        camera.target = playerPos;
        camera.position = (Vector3){playerPos.x, playerPos.y + 10.0f, playerPos.z + 20.0f};


        // --- 2. Draw (ציור בלבד) ---
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);
        DrawGrid(1000, 2.0f);
        DrawPlayer(player);
        DrawEnemy(enemy);
        EndMode3D();
        DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadPlayer(player);
    CloseWindow();
}