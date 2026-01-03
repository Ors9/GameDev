#ifndef ASSETS_MANAGER_H
#define ASSETS_MANAGER_H

#include "raylib.h"
#include "raymath.h"
#include "game_types.h"



typedef struct CharacterResources CharacterResources;

typedef struct AssetManager AssetManager;


void UpdateModelRotate( CharacterResources *resources, float rotation);
void InitCharacterResources(AssetManager *assets);
void UnloadAssetsManager(AssetManager *assets);
ModelAnimation * GetAnimationForState( CharacterResources *resources, PlayerAnimationState state);
Model GetModel(CharacterResources *res);
CharacterResources * GetCharacterRescource(AssetManager * asset , CharacterClass selected_class);
AssetManager *  InitAssetManager();
#endif // ASSETS_MANAGER_H