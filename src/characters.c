#include "characters.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assets_manager.h>
#include <game_state.h>
#include "raymath.h"

static bool CheckMovementInput();
static PlayerAnimationState DeterminePlayerAnimationState(Player *player);
static bool IsActionAnimation(PlayerAnimationState state);
static void UpdatePlayerLogicBaseOnState(Player *player);
static void CalculateRotation(Player *player, Vector3 direction);

typedef struct CharacterStats
{
    int attack;
    int health;
    int defense;
    float speed;
} CharacterStats;

struct Player
{
    CharacterResources *assets;
    CharacterStats *stats;
    CharacterClass classtype;
    Vector3 position;
    int currentHealth;

    PlayerAnimationState currentState;
    float animTime;
    int animIndex;
    float rotation;
    BoundingBox bounds;
};



int GetMaxHealth(Player *p){
    return p->stats->health;
}

Player *InitPlayer(CharacterClass selectedClass, AssetManager *asset)
{
    Player *player = malloc(sizeof(Player));
    if (player == NULL)
    {
        printf("Failed to allocate memory for Player.\n");
        exit(1);
    }

    player->assets = GetCharacterRescource(asset, selectedClass);

    player->classtype = selectedClass;
    int tempCount = 0;
    player->currentState = PLAYER_IDLE;
    player->animIndex = 0;
    player->animTime = 0;

    player->stats = malloc(sizeof(CharacterStats));
    if (player->stats == NULL)
    {
        printf("Failed to allocate memory for CharacterStats.\n");
        exit(1);
    }
    player->stats->attack = 10;
    player->stats->health = 100;
    player->stats->defense = 5;
    player->currentHealth = 100;
    player->position = (Vector3){100, 1, 100}; // שמנו ב-Y=1 כדי שיעמוד על הרשת

    return player;
}


int GetCurrentHealth(Player * p){
    return p->currentHealth;
}


CharacterStats * GetCharacterStats(Player * p){
    return p->stats;
}


static PlayerAnimationState DeterminePlayerAnimationState(Player *player)
{
    PlayerAnimationState currState = player->currentState;
    bool isMoving = CheckMovementInput();

    if (player->currentHealth <= 0)
    {
        return PLAYER_DIE;
    }

    if (IsKeyDown(KEY_THREE))
    {
        return PLAYER_SWIPE;
    }

    if (IsKeyDown(KEY_TWO))
    {
        return PLAYER_JUMP_ATTACK;
    }

    if (IsKeyDown(KEY_ONE))
    {
        return PLAYER_PUNCH;
    }

    if (IsKeyDown(KEY_SPACE))
    {
        return PLAYER_JUMP;
    }

    if (IsKeyDown(KEY_LEFT_SHIFT) && isMoving)
    {
        return PLAYER_RUN;
    }

    if (isMoving)
    {
        return PLAYER_WALK;
    }

    if (IsKeyDown(KEY_FOUR))
    {
        return PLAYER_ROAR;
    }

    if (IsKeyDown(KEY_FIVE))
    {
        return PLAYER_FLEX;
    }

    return PLAYER_IDLE;
}

static bool CheckMovementInput()
{

    return (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP) ||
            IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN) ||
            IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT) ||
            IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT));
}

// פונקציה שבודקת האם המצב הנוכחי הוא אנימציית פעולה (One-shot)
static bool IsActionAnimation(PlayerAnimationState state)
{
    switch (state)
    {
    case PLAYER_JUMP:
    case PLAYER_PUNCH:
    case PLAYER_SWIPE:
    case PLAYER_JUMP_ATTACK:
    case PLAYER_FLEX:
    case PLAYER_ROAR:
        return true; // זו אנימציית פעולה

    default:
        return false; // כל השאר (IDLE, WALK, RUN) הן אנימציות לופ
    }
}

Vector3 GetPlayerPosition(Player *player)
{
    return player->position;
}

static void UpdatePlayerAnimation(Player *player, float deltaTime)
{

    PlayerAnimationState currState = player->currentState;

    float animSpeed = (player->currentState == PLAYER_IDLE) ? 30.0f : 60.0f;
    player->animTime += deltaTime * animSpeed;

    ModelAnimation *currentAnim = GetAnimationForState(player->assets, player->currentState);

    if (player->animTime >= currentAnim->frameCount)
    {

        player->animTime = 0;
        if (IsActionAnimation(currState) == true)
        {
            player->currentState = PLAYER_IDLE;
        }
    }

    if (player->classtype == MONSTER_CHAR)
    {
        return;
    }

    UpdateModelAnimation(GetModel(player->assets), currentAnim[0], (int)player->animTime);
}

void UpdatePlayer(Player *player, float deltaTime, GameState *gs)
{
    // 1. בדיקה האם השחקן נעול באנימציית "פעולה"
    bool isLocked = AnimationController(player);

    if (GetMainGameState(gs) != STATE_GAMEPLAY)
    {
        return;
    }

    if (!isLocked)
    {
        // 2. קביעת המצב החדש לפי קלט
        PlayerAnimationState newState = DeterminePlayerAnimationState(player);

        // 3. אם המצב השתנה, מאפסים טיימר
        if (newState != player->currentState)
        {
            player->currentState = newState;
            player->animTime = 0;
        }

        // 4. עדכון מהירות לפי המצב
        UpdatePlayerLogicBaseOnState(player);
    }

    // 5. תנועה פיזית (הפונקציה כבר בודקת בתוכה אם אפשר לזוז)
    MovingPlayer(player, deltaTime);

    // 6. עדכון פריימים של אנימציה
    UpdatePlayerAnimation(player, deltaTime);
}

static void UpdatePlayerLogicBaseOnState(Player *player)
{
    switch (player->currentState)
    {
    case PLAYER_RUN:
        player->stats->speed = 30.0f;
        break;
    case PLAYER_WALK:
        player->stats->speed = 20.0f;
        break;
    // כל מצבי התקיפה מאפשרים תנועה קלה (Combat Strafe)
    case PLAYER_PUNCH:
    case PLAYER_SWIPE:
    case PLAYER_ROAR:
    case PLAYER_FLEX:
        player->stats->speed = 8.0f;
        break;
    // מצבים שבהם המוטנט חייב לעמוד במקום
    case PLAYER_IDLE:
    case PLAYER_DIE:
    case PLAYER_JUMP_ATTACK:
        player->stats->speed = 0.0f;
        break;
    case PLAYER_JUMP:
        // TODO לעבוד על זה!!!
        break;
    }
}

static void CalculateRotation(Player *player, Vector3 direction)
{
    if (direction.x == 0 && direction.z == 0)
    {
        return;
    }

    player->rotation = atan2f(direction.x, direction.z) * RAD2DEG;

    UpdateModelRotate(player->assets, player->rotation);
}

static void DebugDrawChar(Player *player, float scale)
{
    // --- 1. חישוב וציור ה-Bounding Box ---
    BoundingBox box = GetModelBoundingBox(GetModel(player->assets));

    // החלת Scale
    box.min.x *= scale;
    box.min.y *= scale;
    box.min.z *= scale;
    box.max.x *= scale;
    box.max.y *= scale;
    box.max.z *= scale;

    // הזזה למיקום השחקן (World Space)
    box.min.x += player->position.x;
    box.min.y += player->position.y;
    box.min.z += player->position.z;
    box.max.x += player->position.x;
    box.max.y += player->position.y;
    box.max.z += player->position.z;

    // ציור התיבה באדום
    DrawBoundingBox(box, RED);

    // --- 2. ציור שלד המודל (Wireframe) ---
    // זה יעזור לנו לראות אם המודל קיים אבל שקוף/בלי טקסטורה
    DrawModelWires(GetModel(player->assets), player->position, scale, LIME); // ירוק זוהר כדי להבדיל מהתיבה

    // --- 3. ציור צירים (Gizmo) במרכז השחקן ---
    float axisLength = 5.0f * (scale / 10.0f);                                                                               // אורך ציר פרופורציונלי ל-Scale
    DrawLine3D(player->position, (Vector3){player->position.x + axisLength, player->position.y, player->position.z}, RED);   // X
    DrawLine3D(player->position, (Vector3){player->position.x, player->position.y + axisLength, player->position.z}, GREEN); // Y
    DrawLine3D(player->position, (Vector3){player->position.x, player->position.y, player->position.z + axisLength}, BLUE);  // Z
}

void DrawPlayer(Player *player)
{
    float currentScale = 1.0f;

    if (player->classtype == MONSTER_CHAR)
    {
        currentScale = 3.0f;
    }
    else if (player->classtype == MUTANT_CHAR)
    {
        currentScale = 3.0f;
    }

    // ציור המודל האמיתי
    DrawModel(GetModel(player->assets), player->position, currentScale, WHITE);

    if (player->classtype != MUTANT_CHAR)
    {
        DebugDrawChar(player, currentScale);
    }
}

bool MovingPlayer(Player *player, float deltaTime)
{
    if (player->currentState == PLAYER_JUMP_ATTACK)
    {
        return false;
    }

    bool moving = false;
    Vector3 direction = (Vector3){0, 0, 0};
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        direction.z -= 1.0f;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        direction.z += 1.0f;
    }

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        direction.x -= 1.0f;
    }

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        direction.x += 1.0f;
    }

    float lengthSq = direction.x * direction.x + direction.z * direction.z;
    if (lengthSq > 0)
    {
        direction = Vector3Normalize(direction);
        CalculateRotation(player, direction);
        float currentSpeed = player->stats->speed;
        Vector3 moveVec = Vector3Scale(direction, currentSpeed * deltaTime);
        player->position = Vector3Add(player->position, moveVec);

        return true;
    }

    return false;
}

bool AnimationController(Player *player)
{
    PlayerAnimationState currState = player->currentState;
    ModelAnimation *currentAnim = GetAnimationForState(player->assets, player->currentState);
    if (IsActionAnimation(currState) == true)
    {
        if (player->animTime < currentAnim->frameCount)
        {
            return true;
        }
    }
    return false;
}

void UnloadPlayer(Player *player)
{
    if (player == NULL)
        return; // הגנה: אל תנסה לשחרר מצביע ריק

    if (player->stats != NULL)
    {
        free(player->stats);
        player->stats = NULL; // ניקוי המצביע לאחר השחרור
    }

    free(player);
    player = NULL;
    printf("Player memory cleared successfully.\n");
}