#include "player.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

static bool CheckMovementInput();
static PlayerAnimationState DeterminePlayerAnimationState(Player *player);
static bool IsActionAnimation(PlayerAnimationState state);
static void UpdatePlayerLogicBaseOnState(Player *player);
static void CalculateRotation(Player *player, Vector3 direction);

struct Player
{
    Vector3 position;
    float speed;
    Model Pmodel;
    ModelAnimation animations[ANIM_COUNT];
    PlayerAnimationState currentState;
    float animTime;
    int animIndex;
    float rotation;

    float health;
    float attack;
};

Player *InitPlayer()
{
    Player *player = malloc(sizeof(Player));
    if (player == NULL)
    {
        printf("Failed to allocate memory for Player.\n");
        exit(1);
    }

    int tempCount = 0;
    const char *dir = "assets/model/Player/Mutant/";
    const char *modelsPath[ANIM_COUNT] = {
        "Idle.glb",                   // 0: PLAYER_IDLE
        "Walk.glb",                   // 1: PLAYER_WALK
        "Run.glb",                    // 2: PLAYER_RUN
        "Punch.glb",                  // 3: PLAYER_PUNCH
        "Swipe.glb",                  // 4: PLAYER_SWIPE
        "Mutant_Jumping.glb",         // 5: PLAYER_JUMP
        "Jump_Attack.glb",            // 6: PLAYER_JUMP_ATTACK
        "Roar.glb",                   // 7: PLAYER_ROAR
        "mutant_flexing_muscles.glb", // 8: PLAYER_FLEX
        "Death.glb"                   // 9: PLAYER_DIE
    };

    player->currentState = PLAYER_IDLE;
    player->animIndex = 0;
    player->Pmodel = LoadModel(TextFormat("%sIdle.glb", dir));

    for (int i = 0; i < ANIM_COUNT; i++)
    {
        const char *fullPath = TextFormat("assets/model/Player/Mutant/%s", modelsPath[i]);
        int animsCount = 0;
        ModelAnimation *loadedAnims = LoadModelAnimations(fullPath, &animsCount);

        // בדיקת בטיחות: האם המצביע תקין והאם יש לפחות אנימציה אחת?
        if (loadedAnims != NULL && animsCount > 0)
        {
            player->animations[i] = loadedAnims[0];
            printf("Loaded: %s, BoneCount: %d\n", modelsPath[i], player->animations[i].boneCount);
        }
        else
        {
            printf("WARNING: Failed to load animation: %s. Using Idle as fallback.\n", modelsPath[i]);
            player->animations[i] = player->animations[PLAYER_IDLE]; // Fallback כדי שלא יקרוס
        }
    }

    player->animTime = 0;
    player->attack = 10;
    player->health = 100;
    player->speed = 20;

    player->position = (Vector3){100, 1, 100}; // שמנו ב-Y=1 כדי שיעמוד על הרשת

    return player;
}

static PlayerAnimationState DeterminePlayerAnimationState(Player *player)
{
    PlayerAnimationState currState = player->currentState;
    bool isMoving = CheckMovementInput();

    if (player->health <= 0)
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

Vector3 GetPlayerPosition(Player *player){
    return player->position;
}

static void UpdatePlayerAnimation(Player *player, float deltaTime)
{

    PlayerAnimationState currState = player->currentState;

    float animSpeed = (player->currentState == PLAYER_IDLE) ? 30.0f : 60.0f;
    player->animTime += deltaTime * animSpeed;

    ModelAnimation currentAnim = player->animations[player->currentState];
    if (player->animTime >= currentAnim.frameCount)
    {
        player->animTime = 0;
        if (IsActionAnimation(currState) == true)
        {
            player->currentState = PLAYER_IDLE;
        }
    }

    UpdateModelAnimation(player->Pmodel, currentAnim, (int)player->animTime);
}


void UpdatePlayer(Player *player, float deltaTime) {
    // 1. בדיקה האם השחקן נעול באנימציית "פעולה"
    bool isLocked = AnimationController(player);

    if (!isLocked) {
        // 2. קביעת המצב החדש לפי קלט
        PlayerAnimationState newState = DeterminePlayerAnimationState(player);
        
        // 3. אם המצב השתנה, מאפסים טיימר
        if (newState != player->currentState) {
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
        player->speed = 30.0f;
        break;
    case PLAYER_WALK:
        player->speed = 20.0f;
        break;
    // כל מצבי התקיפה מאפשרים תנועה קלה (Combat Strafe)
    case PLAYER_PUNCH:
    case PLAYER_SWIPE:
    case PLAYER_ROAR:
    case PLAYER_FLEX:
        player->speed = 8.0f;
        break;
    // מצבים שבהם המוטנט חייב לעמוד במקום
    case PLAYER_IDLE:
    case PLAYER_DIE:
    case PLAYER_JUMP_ATTACK:
        player->speed = 0.0f;
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
    player->Pmodel.transform = MatrixRotateY(player->rotation * DEG2RAD);
}

void DrawPlayer(Player * player)
{
    DrawModel(player->Pmodel, player->position, 3.0f, WHITE);
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

bool AnimationController(Player *player)
{
    PlayerAnimationState currState = player->currentState;

    if (IsActionAnimation(currState) == true)
    {
        if (player->animTime < player->animations[player->currentState].frameCount)
        {
            return true;
        }
    }
    return false;
}


void UnloadPlayer(Player * player)
{
    UnloadModel(player->Pmodel);
    free(player);
}