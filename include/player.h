#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "raymath.h"

typedef enum
{
    PLAYER_IDLE = 0,
    PLAYER_WALK,
    PLAYER_RUN,
    PLAYER_PUNCH,
    PLAYER_SWIPE,
    PLAYER_JUMP,
    PLAYER_JUMP_ATTACK,
    PLAYER_ROAR,
    PLAYER_FLEX, // mutant_flexing_muscles.glb
    PLAYER_DIE,  // Death.glb
    ANIM_COUNT   // הגודל הסופי של המערך (10)
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
void UpdatePlayerAnimation(Player *player, float deltaTime);
void CalculateRotation(Player *player, Vector3 direction);
bool CheckMovementInput();
PlayerState DeterminePlayerState(Player *player);
void UpdatePlayerLogicBaseOnState(Player *player);

#endif // PLAYER_H
