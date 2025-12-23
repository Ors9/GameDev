#include "player.h"

void InitPlayer(Player *player)
{
    player->po.height = 1;
    player->po.width = 1;
    player->po.length = 1;
    player->po.color = RED;

    player->attack = 10;
    player->health = 100;
    player->speed = 20;

    player->position = (Vector3){100, 1, 100}; // שמנו ב-Y=1 כדי שיעמוד על הרשת
}

void MovingPlayer(Player *player, float deltaTime)
{
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
        player->position.z -= player->speed * deltaTime;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
        player->position.z += player->speed * deltaTime;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        player->position.x -= player->speed * deltaTime;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        player->position.x += player->speed * deltaTime;
}