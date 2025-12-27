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

void UpdateGameCamera(GameCamera *gCam, Player *player, float deltaTime)
{
    Vector3 playerPos = GetPlayerPosition(player);
    Camera3D camera = gCam->raylibCam;

    // 1. המצלמה מסתכלת תמיד למרכז השחקן
    gCam->raylibCam.target = playerPos;

    // 2. המצלמה תמיד נמצאת בדיוק 10 יחידות מעל ו-20 יחידות מאחורי השחקן
    gCam->raylibCam.position = (Vector3){playerPos.x, playerPos.y + 10.0f, playerPos.z + 20.0f};
}

void UnloadGameCamera(GameCamera *gCam)
{
    free(gCam);
}