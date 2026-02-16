#ifndef ASSETS_MANAGER_H
#define ASSETS_MANAGER_H

#include "raylib.h"
#include "raymath.h"
#include "game_types.h"



typedef struct CharacterResources CharacterResources;
typedef struct EnvironmentResources EnvironmentResources;


typedef struct AssetManager AssetManager;


EnvironmentResources * GetEnivormentResources(AssetManager * asset);
void UnloadAssetsManager(AssetManager *assets);
ModelAnimation * GetAnimationForState( CharacterResources *resources, PlayerAnimationState state);

Model* GetModelPtr(CharacterResources *res);
Model GetModel(CharacterResources *res);
CharacterResources * GetCharacterRescource(AssetManager * asset , CharacterClass selected_class);
AssetManager *  InitAssetManager();
Model GetCharacterModel(CharacterResources* res);

Model GetEnvModelByType(AssetManager *assets, EnivormentResourcesTypes type);
BoundingBox GetEnvBoundsByType(AssetManager *assets, EnivormentResourcesTypes type);
bool IsEnvResourceReady(AssetManager *assets, EnivormentResourcesTypes type);
bool IsCharacterLoaded(CharacterResources* res);
int GetAnimCount(CharacterResources* res, PlayerAnimationState state);
#endif // ASSETS_MANAGER_H