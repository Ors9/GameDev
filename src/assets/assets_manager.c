#include <raylib.h>
#include "characters.h"
#include "assets/assets_manager.h"
#include <stdlib.h>
#include "stdio.h"

static const char *GetFullAnimPath(CharacterClass cls, PlayerAnimationState state);
static const char *GetFullEnivPath(EnivormentResourcesTypes state);
static void InitCharacterResources(AssetManager *assets);
static void InitEnvironmentResources(AssetManager *assets);

struct CharacterResources
{
    Model model;                            // המודל האמיתי ב-VRAM
    ModelAnimation *animations[ANIM_COUNT]; // האנימציות האמיתיות
    int animCounts[ANIM_COUNT];             // כמות האנימציות
    bool isLoaded;
};

struct EnvironmentResources
{
    Model model; // המודל של השטח הסלעי או הדשא
    BoundingBox bounds;
    bool isLoaded;
};

struct AssetManager
{
    EnvironmentResources worldRes[ENIVORMENT_COUNT]; // המשאבים של הסביבה
    CharacterResources classResources[CLASS_COUNT];  // שחקנים
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
    InitEnvironmentResources(assets);

    return assets;
}

static void InitEnvironmentResources(AssetManager *assets)
{
    for (int i = 0; i < ENIVORMENT_COUNT; i++)
    {
        // 1. קבלת הנתיב לפי ה-ID של המשאב
        const char *modelPath = GetFullEnivPath((EnivormentResourcesTypes)i);

        if (modelPath != NULL)
        {
            // 2. טעינה ישירות לתוך המבנה של ה-AssetManager
            assets->worldRes[i].model = LoadModel(modelPath);

            if (assets->worldRes[i].model.meshCount > 0)
            {
                assets->worldRes[i].isLoaded = true;
                printf("Successfully loaded environment model: %s\n", modelPath);
                assets->worldRes[i].bounds = GetModelBoundingBox(assets->worldRes[i].model);
            }
            else
            {
                assets->worldRes[i].isLoaded = false;
                printf("Failed to load environment model: %s\n", modelPath);
            }
        }
    }
}

Model GetEnvModelByType(AssetManager *assets, EnivormentResourcesTypes type)
{
    if (assets == NULL || !assets->worldRes[type].isLoaded)
        return (Model){0};
    return assets->worldRes[type].model;
}

BoundingBox GetEnvBoundsByType(AssetManager *assets, EnivormentResourcesTypes type)
{
    if (assets == NULL)
        return (BoundingBox){0};
    return assets->worldRes[type].bounds;
}

bool IsEnvResourceReady(AssetManager *assets, EnivormentResourcesTypes type)
{
    return (assets != NULL && assets->worldRes[type].isLoaded);
}

static const char *GetFullEnivPath(EnivormentResourcesTypes type)
{
    char *basePath = "assets/model/Enivorment/";
    switch (type)
    {
    case ENIV_WORD_TERRIAN:
        return TextFormat("%s%s", basePath, "rocky_terrain_02_4k.gltf");
    case ENIV_FLOOR_BLOCK:
        return TextFormat("%s%s", basePath, "stone_floor_1x1.glb");
    case ENIV_ROCK_A:
        return TextFormat("%s%s", basePath, "rock_sharp.glb");
    case ENIV_TREE_PINE:
        return TextFormat("%s%s", basePath, "tree_pine.glb");
    default:
        return NULL;
    }
}

CharacterResources *GetCharacterRescource(AssetManager *asset, CharacterClass selected_class)
{
    return &asset->classResources[selected_class];
}

EnvironmentResources *GetEnivormentResources(AssetManager *asset)
{
    return asset->worldRes;
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

static void InitCharacterResources(AssetManager *assets)
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

    for (int i = 0; i < ENIVORMENT_COUNT; i++)
    {
        if (assets->worldRes[i].isLoaded)
        {
            // שחרור המודל (כולל ה-Meshes וה-Materials שלו)
            UnloadModel(assets->worldRes[i].model);

            assets->worldRes[i].isLoaded = false;
        }
    }
    free(assets);
    assets = NULL;
    printf("Assets memory cleared successfully.\n");
}
