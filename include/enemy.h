#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "player.h"
#include "stdlib.h"
#include <stdio.h>

typedef struct Enemy Enemy;


Enemy* InitEnemy(); 
void UpdateEnemy(Enemy *enemy, Player *player, float deltaTime);
void DrawEnemy(Enemy *enemy); 
void UnloadEnemy(Enemy *enemy);

#endif // ENEMY_H
