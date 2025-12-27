#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "raymath.h"



typedef enum PlayerAnimationState
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
    ANIM_COUNT   // הגודל הסופי של המערך
} PlayerAnimationState;


typedef struct Player Player;



// הצהרה על הפונקציות (בלי המימוש שלהן)
Player * InitPlayer();
bool MovingPlayer(Player *player, float deltaTime);
void DrawPlayer(Player * player);
void UpdatePlayer(Player *player, float deltaTime);
bool AnimationController(Player *player);
Vector3 GetPlayerPosition(Player *player);
void UnloadPlayer(Player * player);

#endif // PLAYER_H
