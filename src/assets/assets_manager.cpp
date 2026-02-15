#include <raylib.h>
#include "characters.h"
#include "assets/assets_manager.h"
#include <stdlib.h>
#include "stdio.h"
#include "raymath.h"

static const char *GetFullAnimPath(CharacterClass cls, PlayerAnimationState state);
static const char *GetFullEnivPath(EnivormentResourcesTypes state);
static void InitCharacterResources(AssetManager *assets);
static void InitEnvironmentResources(AssetManager *assets);
static void NormalizeModelScale(CharacterResources *res);

struct CharacterResources
{
    Model model;                            // המודל האמיתי ב-VRAM
    ModelAnimation *animations[ANIM_COUNT]; // האנימציות האמיתיות
    int animCounts[ANIM_COUNT];             // כמות האנימציות
    Matrix baseTransform;
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
static void NormalizeModelScale(CharacterResources *res)
{
    BoundingBox bb = GetModelBoundingBox(res->model);
    float originalHeight = bb.max.y - bb.min.y;
    if (originalHeight <= 0)
        return;

    float scaleFactor = 1.0f / originalHeight;
    float offsetY = -bb.min.y;

    // סדר פעולות נכון ב-Raylib:
    // 1. קודם מזיזים את הרגליים ל-0 (Translate)
    // 2. אז מגדילים פי 100 (Scale)
    Matrix translation = MatrixTranslate(0, offsetY, 0);
    Matrix scaling = MatrixScale(scaleFactor, scaleFactor, scaleFactor);

    // ב-Raylib: Scale(Translate(...))
    res->baseTransform = MatrixMultiply(scaling, translation);
    res->model.transform = res->baseTransform;
    printf("[ASSETS] Warrok Normalized: Original height %.2f, Scale Factor %.4f\n", originalHeight, scaleFactor);
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
    return &asset->classResources[selected_class];
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
        return "assets/model/Player/Warrok/Warrok.glb";
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

            return TextFormat("%s%s", basePath, "Jumping.glb");
        case PLAYER_JUMP_ATTACK:
            return TextFormat("%s%s", basePath, "JumpAttack.glb");
        case PLAYER_ROAR:
            return TextFormat("%s%s", basePath, "Roar.glb");
        case PLAYER_FLEX:
            // ופה השם היה ארוך מאוד
            return TextFormat("%s%s", basePath, "FlexMuscles.glb");
        case PLAYER_DIE:
            return TextFormat("%s%s", basePath, "Dying.glb");
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
        // 1. טעינת המודל (הגוף/Mesh) מהקובץ הייעודי
        const char *meshPath = GetFullMeshPath((CharacterClass)i);
        assets->classResources[i].model = LoadModel(meshPath);

        // בדיקת תקינות - האם המודל מכיל גוף?
        if (assets->classResources[i].model.meshCount > 0)
        {
            assets->classResources[i].isLoaded = true;
            NormalizeModelScale(&assets->classResources[i]);
            printf("[ASSETS] Successfully loaded Mesh for class %d\n", i);
        }
        else
        {
            assets->classResources[i].isLoaded = false;
            printf("[ASSETS] WARNING: No Mesh found for class %d at %s\n", i, meshPath);
        }

        // 2. לולאת טעינת האנימציות (הקבצים הקלים בלי ה-Skin)
        for (int j = 0; j < ANIM_COUNT; j++)
        {
            const char *animPath = GetFullAnimPath((CharacterClass)i, (PlayerAnimationState)j);
            if (animPath != nullptr)
            {
                // Raylib מלביש את האנימציה על המודל שטענו למעלה לפי שמות העצמות
                assets->classResources[i].animations[j] = LoadModelAnimations(animPath, &assets->classResources[i].animCounts[j]);
            }
        }
    }
}

void UpdateModelRotate(CharacterResources *resources, float rotation)
{
    Matrix rotationMat = MatrixRotateY(rotation * DEG2RAD);
    
    // סדר קריטי: Rotation * BaseTransform
    // זה מבטיח שהיא קודם תהיה בגודל 1.0 וב-0, ואז תסתובב סביב הציר שלה
    resources->model.transform = MatrixMultiply(rotationMat, resources->baseTransform);
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