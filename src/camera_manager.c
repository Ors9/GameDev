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
        exit(1);

    // הגדרות בסיסיות של Raylib
    gCam->raylibCam.fovy = 45.0f;
    gCam->raylibCam.up = (Vector3){0.0f, 1.0f, 0.0f};
    gCam->raylibCam.projection = CAMERA_PERSPECTIVE;

    // אתחול מיקום ראשוני קרוב לאזור שהשחקן מתחיל (100, 225, 100)
    gCam->raylibCam.position = (Vector3){100.0f, 235.0f, 120.0f};
    gCam->raylibCam.target = (Vector3){100.0f, 225.0f, 100.0f};

    gCam->offsetDistance = 25.0f; // קצת יותר מרחק כדי לראות את הסביבה
    gCam->height = 12.0f;
    gCam->smoothness = 10.0f; // מהירות הרדיפה של המצלמה

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
        gCam = NULL;
        printf("Camera memory cleared.\n");
    }
}