#ifndef MAP_MANAGER_H
#define MAP_NAMAGER_H

#include "game_types.h"
#include <raylib.h>

typedef struct MapObject MapObject;
typedef struct GameMap GameMap;
typedef enum GameMapID GameMapID;
typedef struct AssetManager AssetManager;


MapObject InitMapObject(EnivormentResourcesTypes modelType, Vector3 position, float scale);
GameMap *InitGameMap(GameMapID mapId);
void UnloadGameMap(GameMap *map);
void AddObjectToMap(GameMap *map, EnivormentResourcesTypes type, Vector3 pos, float scale);
void DrawMap(GameMap *map, AssetManager *assets);
float GetMapHeightAt(GameMap *map, AssetManager *assets, Vector3 position);

// --- Getters עבור GameMap ---
MapObject *GetMapObjects(GameMap *map);
int GetMapObjectCount(GameMap *map);
float GetMapLength(GameMap *map);
float GetMapLaneWidth(GameMap *map);

// --- Getters עבור MapObject ---
EnivormentResourcesTypes GetMapObjectType(MapObject *obj);
Vector3 GetMapObjectPosition(MapObject *obj);
float GetMapObjectScale(MapObject *obj);
void PrintMapInfo(GameMap *map);


#endif //MAP_MANAGER_H