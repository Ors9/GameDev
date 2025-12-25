#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "raymath.h"

typedef struct Player
{
    Vector3 position;
    float speed;
    Model Pmodel;
    ModelAnimation *modelAnim;
    float animTime;
    int animsCount;
    int countCurrModelAnimation;
    float health;
    float attack;
} Player;

// הצהרה על הפונקציות (בלי המימוש שלהן)
void InitPlayer(Player *player);
bool MovingPlayer(Player *player, float deltaTime);
void DrawPlayer(Player player);
void UpdatePlayerAnimation(Player * player ,bool isMoving ,float deltaTime);

#endif // PLAYER_H
