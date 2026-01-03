#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "raylib.h"
#include "characters.h"


typedef struct GameCamera GameCamera;

GameCamera* InitGameCamera();
void UpdateGameCamera(GameCamera *gCam, Player *player, float deltaTime);
Camera3D GetRaylibCamera(GameCamera *gCam); 
void UnloadGameCamera(GameCamera *gCam);
void MyUpdateCamera(GameCamera *gCam, Vector3  pos, float deltaTime);

#endif