#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "player.h"

typedef struct EnemyObject {
    float height;
    float width;
    float length;
    Color color;
} EnemyObject;

typedef struct Enemy
{
    Vector3 position;
    EnemyObject po;
    float speed;
    float health;
    float attack;
} Enemy;

void DrawEnemy(Enemy enemy);
void InitEnemy(Enemy *enemy);
void UpdateEnemy(Enemy *enemy, Player *player, float deltaTime);



#endif // ENEMY_H
