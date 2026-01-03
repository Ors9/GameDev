#include "camera_manager.h"
#include <stdio.h>
#include <stdlib.h>

struct GameCamera
{
    Camera3D raylibCam;
    float offsetDistance;
    float height;
    float smoothness;
};

GameCamera *InitGameCamera()
{
    GameCamera *gCam = malloc(sizeof(GameCamera));
    if (gCam == NULL)
    {
        printf("Failed to allocate memory for GameCamera.\n");
        exit(1);
    }

    // --- אתחול חובה של נתוני Raylib ---
    gCam->raylibCam.fovy = 45.0f;
    gCam->raylibCam.up = (Vector3){0.0f, 1.0f, 0.0f};
    gCam->raylibCam.projection = CAMERA_PERSPECTIVE;
    gCam->raylibCam.position = (Vector3){0, 10, 20}; // מיקום התחלתי זמני
    gCam->raylibCam.target = (Vector3){0, 0, 0};

    // --- אתחול הגדרות המעקב ---
    gCam->offsetDistance = 20.0f; // מרחק מהשחקן
    gCam->height = 10.0f;         // גובה מעל השחקן
    gCam->smoothness = 5.0f;      // ככל שהמספר גבוה יותר, המעקב מהיר יותר

    return gCam;
}

Camera3D GetRaylibCamera(GameCamera *gCam)
{
    return gCam->raylibCam;
}

void MyUpdateCamera(GameCamera *gCam, Vector3 targetPos, float deltaTime)
{

    gCam->raylibCam.target = targetPos;

    gCam->raylibCam.position = (Vector3){
        targetPos.x,
        targetPos.y + gCam->height,
        targetPos.z + gCam->offsetDistance};
}

void UpdateGameCamera(GameCamera *gCam, Player *player, float deltaTime)
{
    Vector3 playerPos = GetPlayerPosition(player);

    // קוראים לפונקציה הכללית שלנו
    MyUpdateCamera(gCam, playerPos, deltaTime);
}

void UnloadGameCamera(GameCamera *gCam)
{
    if (gCam != NULL)
    {
        free(gCam);
        printf("Camera memory cleared.\n");
    }
}