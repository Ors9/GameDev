#include "player.h"
#include <stdio.h>
#include <math.h>


/**
 * To DO: 
 *  Add character assets in C:\Users\or8sa\Desktop\GameProjectAssets\שחקן
 * להמיר אותם לפורמט glb ולשלב בקוד
 * זה הולך לדרוש הרבה עבודה
 */

void InitPlayer(Player *player)
{
    int tempCount = 0;
    player->currentState = PLAYER_IDLE;
    player->animIndex = 0;
    player->Pmodel = LoadModel("assets/model/Player/Run.glb");
    player->animations[PLAYER_IDLE] = LoadModelAnimations("assets/model/Player/Run.glb", &tempCount)[0];
    player->animations[PLAYER_RUN] = LoadModelAnimations("assets/model/Player/FastRun.glb", &tempCount)[0];
    player->animations[PLAYER_JUMP] = LoadModelAnimations("assets/model/Player/Jumping.glb", &tempCount)[0];
    // אם אין לך עדיין קובץ Attack, תוכל להעתיק זמנית את ה-Idle כדי שלא יקרוס
    player->animations[PLAYER_ATTACK] = player->animations[PLAYER_IDLE];

    player->animTime = 0;
    player->attack = 10;
    player->health = 100;
    player->speed = 20;

    player->position = (Vector3){100, 1, 100}; // שמנו ב-Y=1 כדי שיעמוד על הרשת
}

void UpdatePlayerAnimation(Player *player, bool isMoving, float deltaTime)
{
    PlayerState oldState = player->currentState;
    PlayerState newState = isMoving ? PLAYER_RUN : PLAYER_IDLE;

    if (oldState != newState)
    {
        player->currentState = newState;
        player->animTime = 0;
    }
    
    float animSpeed = (player->currentState == PLAYER_IDLE) ? 30.0f : 60.0f;
    player->animTime += deltaTime * animSpeed;

    ModelAnimation currentAnim = player->animations[player->currentState];
    if (player->animTime >= currentAnim.frameCount)
    {
        player->animTime = 0;
    }

    UpdateModelAnimation(player->Pmodel, currentAnim, (int)player->animTime);

}

void CalculateRotation(Player *player, Vector3 direction)
{
    if (direction.x == 0 && direction.z == 0)
    {
        return;
    }

    player->rotation = atan2f(direction.x, direction.z) * RAD2DEG;
    player->Pmodel.transform = MatrixRotateY(player->rotation * DEG2RAD);
}

void DrawPlayer(Player player)
{
    DrawModel(player.Pmodel, player.position, 1.0f, WHITE);
}

bool MovingPlayer(Player *player, float deltaTime)
{
    bool moving = false;
    Vector3 direction = (Vector3){0, 0, 0};
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        direction.z = -1;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        direction.z = 1;
    }

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        direction.x = -1;
    }

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        direction.x = 1;
    }

    Vector3 length;
    Vector3 moveVec;
    if (direction.x != 0 || direction.z != 0)
    {
        CalculateRotation(player, direction);
        moveVec = Vector3Normalize(direction);
        length = Vector3Scale(moveVec, deltaTime * player->speed);
        player->position = Vector3Add(player->position, length);

        return true;
    }

    return false;
}
