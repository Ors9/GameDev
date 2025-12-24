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
    const int screenWidth = 2400;
    const int screenHeight = 1600;

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
        // 1. Update
        float deltaTime = GetFrameTime();
        MovingPlayer(&player, deltaTime);

        camera.target = player.position;
        camera.position = (Vector3){player.position.x, player.position.y + 10.0f, player.position.z + 20.0f};

        // 2. Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);
        DrawGrid(1000, 2.0f);

        DrawPlayer(player);
        DrawEnemy(enemy);

        UpdateEnemy(&enemy, &player, deltaTime);

        EndMode3D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}