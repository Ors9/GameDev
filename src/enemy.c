#include "enemy.h"
#include "raymath.h"


void InitEnemy(Enemy *enemy) {
    // אתחול הנתונים הגרפיים
    enemy->po.height = 1.0f;
    enemy->po.width = 1.0f;
    enemy->po.length = 1.0f;
    enemy->po.color = GREEN;

    // אתחול נתוני המשחק
    enemy->attack = 5.0f;
    enemy->health = 50.0f;
    enemy->speed = 5.0f;

    // מיקום התחלתי
    enemy->position = (Vector3){110.0f, 1.0f, 110.0f};
}



void UpdateEnemy(Enemy *enemy , Player *player , float deltaTime){
    Vector3 playerPos = GetPlayerPosition(player);
    Vector3 direction = Vector3Subtract(playerPos, enemy->position);
    direction = Vector3Normalize(direction);   

    if(Vector3Distance(enemy->position, playerPos) < enemy->po.width/2.0f + 2){
        return;
    }
    
    enemy->position.x = enemy->position.x + (direction.x * enemy->speed * deltaTime);
    enemy->position.z = enemy->position.z + (direction.z * enemy->speed * deltaTime);

}

void DrawEnemy(Enemy enemy){
    DrawCube(enemy.position , enemy.po.width , enemy.po.height , enemy.po.length , enemy.po.color);
}
