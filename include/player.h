#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef struct PlayerObject {
    float height;
    float width;
    float length;
    Color color;
} PlayerObject;

typedef struct Player {
    Vector3 position;
    float speed;
    PlayerObject po;
    float health;
    float attack;
} Player;

// הצהרה על הפונקציות (בלי המימוש שלהן)
void InitPlayer(Player *player);
void MovingPlayer(Player *player, float deltaTime);
void DrawPlayer(Player player);

#endif // PLAYER_H


