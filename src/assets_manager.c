#include <raylib.h>
#include "characters.h"
#include "assets_manager.h"
#include <stdlib.h>
#include "stdio.h"

static const char *GetFullAnimPath(CharacterClass cls, PlayerAnimationState state);

struct CharacterResources
{
    Model model;                            // המודל האמיתי ב-VRAM
    ModelAnimation *animations[ANIM_COUNT]; // האנימציות האמיתיות
    int animCounts[ANIM_COUNT];             // כמות האנימציות
    bool isLoaded;
};

struct AssetManager
{
    CharacterResources classResources[CLASS_COUNT]; // מחסן מרכזי
};

AssetManager *InitAssetManager()
{

    AssetManager *assets = malloc(sizeof(AssetManager));
    if (assets == NULL)
    {
        printf("Failed to allocate memory for AssetManager.\n");
        exit(1);
    }

    InitCharacterResources(assets);

    return assets;
}

CharacterResources *GetCharacterRescource(AssetManager *asset, CharacterClass selected_class)
{
    return &asset->classResources[selected_class];
}

static const char *GetFullAnimPath(CharacterClass cls, PlayerAnimationState state)
{
    char *basePath;
    switch (cls)
    {
    case MUTANT_CHAR:
        basePath = "assets/model/Player/Mutant/";

        switch (state)
        {
        case PLAYER_IDLE:
            return TextFormat("%s%s", basePath, "Idle.glb");
        case PLAYER_WALK:
            return TextFormat("%s%s", basePath, "Walk.glb");
        case PLAYER_RUN:
            return TextFormat("%s%s", basePath, "Run.glb");
        case PLAYER_PUNCH:
            return TextFormat("%s%s", basePath, "Punch.glb");
        case PLAYER_SWIPE:
            return TextFormat("%s%s", basePath, "Swipe.glb");
        case PLAYER_JUMP:
            // שים לב: פה קראת לזה Mutant_Jumping ולא Jump
            return TextFormat("%s%s", basePath, "Mutant_Jumping.glb");
        case PLAYER_JUMP_ATTACK:
            return TextFormat("%s%s", basePath, "Jump_Attack.glb");
        case PLAYER_ROAR:
            return TextFormat("%s%s", basePath, "Roar.glb");
        case PLAYER_FLEX:
            // ופה השם היה ארוך מאוד
            return TextFormat("%s%s", basePath, "mutant_flexing_muscles.glb");
        case PLAYER_DIE:
            return TextFormat("%s%s", basePath, "Death.glb");
        default:
            return NULL;
        }
        break;
    case MONSTER_CHAR:
        basePath = "assets/model/Player/Monster/";
        return TextFormat("%s%s", basePath, "character.glb");
    default:
        return NULL;
    }

    return NULL;
}

void InitCharacterResources(AssetManager *assets)
{
    for (int i = 0; i < CLASS_COUNT; i++)
    {
        const char *modelPath = GetFullAnimPath(i, PLAYER_IDLE);
        assets->classResources[i].model = LoadModel(modelPath);

        for (int j = 0; j < ANIM_COUNT; j++)
        {

            const char *animPath = GetFullAnimPath(i, j);

            if (animPath != NULL)
            {
                int animsInFile = 0;
                assets->classResources[i].animations[j] = LoadModelAnimations(animPath, &assets->classResources[i].animCounts[j]);
            }
        }
        assets->classResources[i].isLoaded = true;
    }
}

void UpdateModelRotate(CharacterResources *resources, float rotation)
{
    resources->model.transform = MatrixRotateY(rotation * DEG2RAD);
}

Model GetModel(CharacterResources *res)
{
    return res->model;
}

ModelAnimation *GetAnimationForState(CharacterResources *resources, PlayerAnimationState state)
{
    return resources->animations[state];
}

void UnloadAssetsManager(AssetManager *assets)
{
    if (assets == NULL)
        return;

    for (int i = 0; i < CLASS_COUNT; i++)
    {
        // 1. שחרור המודל מהכרטיס הגרפי
        UnloadModel(assets->classResources[i].model);

        // 2. שחרור כל מערכי האנימציות שנטענו
        for (int j = 0; j < ANIM_COUNT; j++)
        {
            if (assets->classResources[i].animations[j] != NULL)
            {
                // Raylib דורשת את המצביע ואת הכמות ששמרנו
                UnloadModelAnimations(assets->classResources[i].animations[j], assets->classResources[i].animCounts[j]);
                assets->classResources[i].animations[j] = NULL;
            }
        }
        assets->classResources[i].isLoaded = false;
    }
}
