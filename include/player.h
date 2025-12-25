#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "raymath.h"

typedef enum {
    PLAYER_IDLE = 0,
    PLAYER_RUN,
    PLAYER_JUMP,
    PLAYER_ATTACK,
    ANIM_COUNT // טריק של מתכנתים כדי לדעת את גודל המערך
} PlayerState;

typedef struct Player
{
    Vector3 position;
    float speed;
    Model Pmodel;
    ModelAnimation animations[ANIM_COUNT];
    PlayerState currentState;
    float animTime;
    int animIndex;
    float rotation;
 
    float health;
    float attack;
} Player;

// הצהרה על הפונקציות (בלי המימוש שלהן)
void InitPlayer(Player *player);
bool MovingPlayer(Player *player, float deltaTime);
void DrawPlayer(Player player);
void UpdatePlayerAnimation(Player * player ,bool isMoving ,float deltaTime);
void CalculateRotation(Player *player , Vector3 direction);

#endif // PLAYER_H
