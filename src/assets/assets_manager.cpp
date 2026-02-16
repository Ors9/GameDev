#include <raylib.h>
#include "characters.h"
#include "assets/assets_manager.h"
#include <stdlib.h>
#include "stdio.h"
#include "raymath.h"
#include <cstring>

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

Model GetCharacterModel(CharacterResources *res)
{
    return res->model;
}

AssetManager *InitAssetManager()
{

    AssetManager *assets = new AssetManager;
    if (assets == nullptr)
    {
        printf("Failed to allocate memory for AssetManager.\n");
        exit(1);
    }
    memset(assets, 0, sizeof(AssetManager));
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

        if (modelPath != nullptr)
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

bool IsCharacterLoaded(CharacterResources *res)
{
    return (res != nullptr && res->isLoaded);
}

// מחזיר את כמות האנימציות שיש למצב ספציפי
int GetAnimCount(CharacterResources *res, PlayerAnimationState state)
{
    if (res == nullptr)
        return 0;
    return res->animCounts[state];
}

Model GetEnvModelByType(AssetManager *assets, EnivormentResourcesTypes type)
{
    if (assets == nullptr || !assets->worldRes[type].isLoaded)
        return (Model){0};
    return assets->worldRes[type].model;
}

BoundingBox GetEnvBoundsByType(AssetManager *assets, EnivormentResourcesTypes type)
{
    if (assets == nullptr)
        return (BoundingBox){0};
    return assets->worldRes[type].bounds;
}

bool IsEnvResourceReady(AssetManager *assets, EnivormentResourcesTypes type)
{
    return (assets != nullptr && assets->worldRes[type].isLoaded);
}

static const char *GetFullEnivPath(EnivormentResourcesTypes type)
{
    char *basePath = "assets/model/Enivorment/";
    switch (type)
    {
    case ENIV_WORD_TERRIAN:
        return TextFormat("%s%s", basePath, "rocky_terrain_02_4k.gltf");

    case ENIV_FLOOR_BLOCK:
        return TextFormat("%s%s", basePath, "floor_material/scene.gltf");
    case ENIV_ROCK_A:
        return TextFormat("%s%s", basePath, "rock_sharp.glb");
    case ENIV_TREE_PINE:
        return TextFormat("%s%s", basePath, "tree_pine.glb");
    default:
        return nullptr;
    }
}

CharacterResources *GetCharacterRescource(AssetManager *asset, CharacterClass selected_class)
{
    // 1. בדיקה שה-AssetManager בכלל קיים
    if (asset == nullptr)
    {
        printf("[ERROR] GetCharacterRescource: asset manager is NULL!\n");
        return nullptr;
    }

    // 2. בדיקה שהאינדקס בתוך הגבולות של המערך
    if (selected_class < 0 || selected_class >= CLASS_COUNT)
    {
        printf("[ERROR] GetCharacterRescource: Invalid class index: %d (Max: %d)\n", selected_class, CLASS_COUNT - 1);
        return nullptr;
    }

    // 3. הדפסת המצביע שנשלח חזרה
    CharacterResources *res = &asset->classResources[selected_class];

    // בדיקה אם המשאב בכלל נטען לפני שמחזירים אותו
    if (!res->isLoaded)
    {
        printf("[WARNING] GetCharacterRescource: Returning resource for class %d, but isLoaded is FALSE!\n", selected_class);
    }

    return res;
}

EnvironmentResources *GetEnivormentResources(AssetManager *asset)
{
    return asset->worldRes;
}

static const char *GetFullMeshPath(CharacterClass cls)
{
    switch (cls)
    {
    case MUTANT_CHAR:
        return "assets/model/Player/Mutant/Mutant.glb";
    case MONSTER_CHAR:
        return "assets/model/Player/Warrok/Mutant.glb";
    default:
        return nullptr;
    }
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
            return nullptr;
        }
        break;
    case MONSTER_CHAR:
        basePath = "assets/model/Player/Warrok/";

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
            return nullptr;
        }
    default:
        return nullptr;
    }

    return nullptr;
}
static void InitCharacterResources(AssetManager *assets)
{
    for (int i = 0; i < CLASS_COUNT; i++)
    {
        CharacterClass cls = (CharacterClass)i;
        const char *meshPath = GetFullMeshPath(cls);

        // בדיקה 1: האם הנתיב קיים בדיסק?
        if (!FileExists(meshPath))
        {
            printf("[INIT ERROR] Class %d: File NOT FOUND at %s\n", i, meshPath);
            assets->classResources[i].isLoaded = false;
            continue; // דלג לקלאס הבא
        }

        assets->classResources[i].model = LoadModel(meshPath);

        if (assets->classResources[i].model.meshCount > 0)
        {
            assets->classResources[i].isLoaded = true;
            printf("[INIT] Mesh loaded for Class %d: %d bones found.\n", i, assets->classResources[i].model.boneCount);
        }
        else
        {
            // כאן המקום שבו המפלצת שלך "נעלמת"
            printf("[INIT ERROR] Class %d: LoadModel failed (0 meshes) for %s\n", i, meshPath);
            assets->classResources[i].isLoaded = false;
            continue;
        }

        // טעינת אנימציות - רק אם המודל נטען בהצלחה
        for (int j = 0; j < ANIM_COUNT; j++)
        {
            const char *animPath = GetFullAnimPath(cls, (PlayerAnimationState)j);
            if (animPath != nullptr && FileExists(animPath))
            {
                int count = 0;
                assets->classResources[i].animations[j] = LoadModelAnimations(animPath, &count);
                assets->classResources[i].animCounts[j] = count;

                if (count > 0)
                {
                    printf("[INIT] Loaded %d anims for State %d\n", count, j);
                }
                else
                {
                    printf("[INIT WARNING] State %d has 0 animations in file %s\n", j, animPath);
                }
            }
            else if (animPath != nullptr)
            {
                printf("[INIT ERROR] Animation file missing: %s\n", animPath);
            }
        }
    }
}

Model GetModel(CharacterResources *res)
{
    return res->model;
}
Model *GetModelPtr(CharacterResources *res)
{
    return &res->model;
}

ModelAnimation *GetAnimationForState(CharacterResources *resources, PlayerAnimationState state)
{
    return resources->animations[state];
}

void UnloadAssetsManager(AssetManager *assets)
{
    if (assets == nullptr)
        return;

    // 1. שחרור דמויות
    for (int i = 0; i < CLASS_COUNT; i++)
    {
        if (assets->classResources[i].isLoaded)
        {
            UnloadModel(assets->classResources[i].model);

            for (int j = 0; j < ANIM_COUNT; j++)
            {
                if (assets->classResources[i].animations[j] != nullptr)
                {
                    // שחרור האנימציות
                    UnloadModelAnimations(assets->classResources[i].animations[j], assets->classResources[i].animCounts[j]);
                    assets->classResources[i].animations[j] = nullptr;
                }
            }
        }
    }

    // 2. שחרור סביבה (רק מה שבאמת נטען)
    for (int i = 0; i < ENIVORMENT_COUNT; i++)
    {
        if (assets->worldRes[i].isLoaded)
        {
            UnloadModel(assets->worldRes[i].model);
        }
    }

    // 3. מחיקת האובייקט עצמו
    delete assets;
    // עדיף להדפיס פעם אחת בסוף ולא בכל שלב
    printf("All assets cleared from GPU and RAM.\n");
}