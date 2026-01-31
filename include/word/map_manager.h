#ifndef MAP_MANAGER_H
#define MAP_NAMAGER_H

#include "game_types.h"
#include "raymath.h"

typedef struct MapObject MapObject;
typedef struct GameMap GameMap;
typedef enum GameMapID GameMapID;


MapObject InitMapObject(EnivormentResourcesTypes modelType, Vector3 position, float scale);
GameMap *InitGameMap(GameMapID mapId);
void UnloadGameMap(GameMap *map);
void AddObjectToMap(GameMap *map, EnivormentResourcesTypes type, Vector3 pos, float scale);

// --- Getters עבור GameMap ---
MapObject *GetMapObjects(GameMap *map);
int GetMapObjectCount(GameMap *map);
float GetMapLength(GameMap *map);
float GetMapLaneWidth(GameMap *map);

// --- Getters עבור MapObject ---
EnivormentResourcesTypes GetMapObjectType(MapObject *obj);
Vector3 GetMapObjectPosition(MapObject *obj);
float GetMapObjectScale(MapObject *obj);


#endif //MAP_MANAGER_H