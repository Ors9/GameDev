#include "raylib.h"
#include "player.h"
#include "enemy.h"

// פונקציית עזר למצלמה (אפשר להשאיר כאן או להוציא לקובץ camera.h בעתיד)
void InitCamera(Camera3D *camera)
{
    camera->fovy = 45.0f;
    camera->up = (Vector3){0.0f, 1.0f, 0.0f};
    camera->projection = CAMERA_PERSPECTIVE;
}

int main()
{
    const int screenWidth = 2000;
    const int screenHeight = 1200;

    InitWindow(screenWidth, screenHeight, "Summoner Game");

    Player player;
    Enemy enemy;
    InitPlayer(&player);
    InitEnemy(&enemy);

    Camera3D camera = {0};
    InitCamera(&camera);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // --- 1. Update (לוגיקה וחישובים) ---
        float deltaTime = GetFrameTime();

        // עדכון אויב)
        UpdateEnemy(&enemy, &player, deltaTime);

        // לוגיקה של השחקן
        bool isMoving = MovingPlayer(&player, deltaTime);
        bool cantMove = AnimationController(&player);

        if (!cantMove)
        {
            PlayerState newState = DeterminePlayerState(&player);
            if (newState != player.currentState)
            {
                player.currentState = newState;
                player.animTime = 0;
            }
            UpdatePlayerLogicBaseOnState(&player);
        }

        UpdatePlayerAnimation(&player, deltaTime);

        // עדכון מצלמה שעוקבת אחרי השחקן
        camera.target = player.position;
        camera.position = (Vector3){player.position.x, player.position.y + 10.0f, player.position.z + 20.0f};

        // --- 2. Draw (ציור בלבד) ---
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);
        DrawGrid(1000, 2.0f);

        DrawPlayer(player);
        DrawEnemy(enemy);

        UpdateEnemy(&enemy, &player, deltaTime);

        EndMode3D();
            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}