#include "enemy.h"
#include "raymath.h"

struct EnemyObject
{
    float height;
    float width;
    float length;
    Color color;
};

struct Enemy
{
    Vector3 position;
    struct EnemyObject po;
    float speed;
    float health;
    float attack;
};

Enemy *InitEnemy()
{
    // הקצאת זיכרון דינמית
    Enemy *enemy = malloc(sizeof(Enemy));
    if (enemy == NULL)
    {
        printf("Failed to allocate memory for Enemy.\n");
        exit(1);
    }

    enemy->po.height = 1.0f;
    enemy->po.width = 1.0f;
    enemy->po.length = 1.0f;
    enemy->po.color = GREEN;

    enemy->attack = 5.0f;
    enemy->health = 50.0f;
    enemy->speed = 5.0f;
    enemy->position = (Vector3){110.0f, 1.0f, 110.0f};

    return enemy;
}
void UpdateEnemy(Enemy *enemy, Player *player, float deltaTime)
{
    if (enemy == NULL || player == NULL)
        return;

    Vector3 playerPos = GetPlayerPosition(player);
    Vector3 direction = Vector3Subtract(playerPos, enemy->position);

    float distance = Vector3Length(direction);
    direction = Vector3Normalize(direction);

    // עצירה לפני התנגשות בשחקן
    if (distance < (enemy->po.width / 2.0f + 1.5f))
    {
        return;
    }

    enemy->position.x += direction.x * enemy->speed * deltaTime;
    enemy->position.z += direction.z * enemy->speed * deltaTime;
}
void DrawEnemy(Enemy *enemy)
{
    DrawCube(enemy->position, enemy->po.width, enemy->po.height, enemy->po.length, enemy->po.color);
}

void UnloadEnemy(Enemy *enemy)
{
    if (enemy != NULL)
    {
        free(enemy);
    }
}