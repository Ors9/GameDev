#include "characters.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "assets/assets_manager.h"
#include <game_state.h>
#include "raymath.h"
#include "word/map_manager.h"
#include <rlgl.h>

static bool CheckMovementInput();
static PlayerAnimationState DeterminePlayerAnimationState(Player *player);
static bool IsActionAnimation(PlayerAnimationState state);
static void UpdatePlayerLogicBaseOnState(Player *player);
static void CalculateRotation(Player *player, Vector3 direction);
static void UpdateY(GameState *gs, Player *player);

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
    CharacterStats stats;
    CharacterClass classtype;
    Vector3 position;
    int currentHealth;

    PlayerAnimationState currentState;
    float animTime;
    int animIndex;
    float rotation;
    BoundingBox bounds;
};

int GetMaxHealth(Player *p)
{
    return p->stats.health;
}

Player *InitPlayer(CharacterClass selectedClass, AssetManager *asset)
{
    Player *player = new Player;
    if (player == nullptr)
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

    player->stats.attack = 10;
    player->stats.health = 100;
    player->stats.defense = 5;
    player->currentHealth = 100;
    player->position = (Vector3){100.0f, 225.0f, 0.0f}; // שמנו ב-Y=1 כדי שיעמוד על הרשת
    player->stats.speed = 0.0f;
    return player;
}

int GetCurrentHealth(Player *p)
{
    return p->currentHealth;
}

CharacterStats *GetCharacterStats(Player *p)
{
    return &p->stats;
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
    // 1. בדיקת הגנה ראשונית - האם המשאבים קיימים וטעונים?
    if (!IsCharacterLoaded(player->assets))
        return;

    // 2. שליפת המודל והאנימציה דרך ה-Getters
    Model *m = GetModelPtr(player->assets);
    ModelAnimation *currentAnim = GetAnimationForState(player->assets, player->currentState);
    int count = GetAnimCount(player->assets, player->currentState);

    if (m->boneCount != currentAnim[0].boneCount)
    {
        printf("CRITICAL ERROR: Bone count mismatch! Model: %d, Anim: %d\n", m->boneCount, currentAnim[0].boneCount);
        return; // עוצרים כאן כדי שלא יקרוס
    }

    // 3. בדיקת בטיחות: האם יש אנימציה תקינה למצב הנוכחי?
    if (currentAnim == nullptr || count == 0)
    {
        // אם אין אנימציה, נחזור ל-IDLE כדי שהשחקן לא "ייתקע"
        if (player->currentState != PLAYER_IDLE)
        {
            player->currentState = PLAYER_IDLE;
            player->animTime = 0;
        }
    }

    // 4. חישוב זמן האנימציה
    float animSpeed = (player->currentState == PLAYER_IDLE) ? 30.0f : 60.0f;
    player->animTime += deltaTime * animSpeed;

    // 5. לולאת האנימציה (Loop) או חזרה ל-IDLE (Action)
    if (player->animTime >= currentAnim[0].frameCount)
    {
        player->animTime = 0;
        if (IsActionAnimation(player->currentState))
        {
            player->currentState = PLAYER_IDLE;
        }
    }

    // 6. הקריאה לעדכון - שימוש במודל שקיבלנו מה-Getter
    // שים לב: 'm' הוא כבר המודל, אז לא צריך 'm.model'
    UpdateModelAnimation(*m, currentAnim[0], (int)player->animTime);
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
    UpdateY(gs, player);

    // 6. עדכון פריימים של אנימציה
    UpdatePlayerAnimation(player, deltaTime);
}

static void UpdateY(GameState *gs, Player *player)
{
    AssetManager *asset = getAssetManager(gs);
    GameMap *map = GetMap(gs);
    float deltaTime = GetFrameTime();

    if (map != nullptr)
    {
        float targetHeight = GetMapHeightAt(map, asset, player->position);

        // שימוש ב-Lerp כדי להחליק את המעבר בין גבהים
        // ה-15.0f הוא "מהירות ההצמדה". ככל שהמספר גבוה יותר, הוא ייצמד מהר יותר
        player->position.y = Lerp(player->position.y, targetHeight, deltaTime * 15.0f);
    }
    else
    {
        player->position.y = Lerp(player->position.y, 220.0f, deltaTime * 15.0f);
    }
}

static void UpdatePlayerLogicBaseOnState(Player *player)
{
    switch (player->currentState)
    {
    case PLAYER_RUN:
        player->stats.speed = 30.0f;
        break;
    case PLAYER_WALK:
        player->stats.speed = 20.0f;
        break;
    // כל מצבי התקיפה מאפשרים תנועה קלה (Combat Strafe)
    case PLAYER_PUNCH:
    case PLAYER_SWIPE:
    case PLAYER_ROAR:
    case PLAYER_FLEX:
        player->stats.speed = 8.0f;
        break;
    // מצבים שבהם המוטנט חייב לעמוד במקום
    case PLAYER_IDLE:
    case PLAYER_DIE:
    case PLAYER_JUMP_ATTACK:
        player->stats.speed = 0.0f;
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
    float currentScale = (player->classtype == MONSTER_CHAR) ? 30.0f : 6.0f;
    Model *model = GetModelPtr(player->assets);

    // ביטול זמני של הסתרת פנים אחוריים - כדי לראות אם המפלצת "הפוכה"
    rlDisableBackfaceCulling();

    DrawModel(*model, player->position, currentScale, WHITE);

    rlEnableBackfaceCulling(); // מחזירים למצב רגיל

    if (player->classtype == MONSTER_CHAR)
        DebugDrawChar(player, currentScale);
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
        float currentSpeed = player->stats.speed;
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
    if (player == nullptr)
        return; // הגנה: אל תנסה לשחרר מצביע ריק

    delete player;
    player = nullptr;
    printf("Player memory cleared successfully.\n");
}