#include "player.h"
#include <stdio.h>

void InitPlayer(Player *player)
{
    const char *playerAsset = "assets/model/Player/Player.glb";
    player->animsCount = 0;
    player->Pmodel = LoadModel(playerAsset);
    player->modelAnim = LoadModelAnimations(playerAsset, &player->animsCount);
    player->countCurrModelAnimation = 0;
    player->animTime = 0;
    player->attack = 10;
    player->health = 100;
    player->speed = 20;
    if (player->modelAnim == NULL)
    {
        printf("ERROR: Could not load animations. Check your file path!\n");
    }
    printf("Number of animations: %d\n", player->animsCount);
    for (int i = 0; i < player->animsCount; i++)
    {
        printf("Anim %d: %s\n", i, player->modelAnim[i].name);
    }
    player->position = (Vector3){100, 1, 100}; // שמנו ב-Y=1 כדי שיעמוד על הרשת
}

void UpdatePlayerAnimation(Player *player, bool isMoving, float deltaTime)
{
    

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
        moveVec = Vector3Normalize(direction);
        length = Vector3Scale(moveVec, deltaTime * player->speed);
        player->position = Vector3Add(player->position, length);
        return true;
    }

    return false;
}
