#include <raylib.h>
#include "word/map_manager.h"
#include "assets/assets_manager.h"
#include "stdlib.h"
#include <stdio.h>
#include <game_state.h>
#include "game_types.h"
#include "word/maps/tuturial_map.h"

struct MapObject
{
    EnivormentResourcesTypes modelType;
    Vector3 position;
    float scale;
};

struct GameMap
{
    GameMapID mapId;
    MapObject *objects; // רשימת כל העצים והסלעים במפה
    int objectCount;
    float mapLength; // כמה ארוך השלב (ציר X)
    float laneWidth; // רוחב המסלול (ציר Z)
};

MapObject InitMapObject(EnivormentResourcesTypes modelType, Vector3 position, float scale)
{
    MapObject obj;
    obj.modelType = modelType;
    obj.position = position;
    obj.scale = scale;
    return obj;
}

void AddObjectToMap(GameMap *map, EnivormentResourcesTypes type, Vector3 pos, float scale)
{
    if (map == NULL)
        return;

    // 1. הגדלת הזיכרון של המערך עבור אובייקט אחד נוסף
    map->objectCount++;
    map->objects = (MapObject *)realloc(map->objects, sizeof(MapObject) * map->objectCount);

    if (map->objects == NULL)
    {
        printf("Error: Failed to reallocate memory for map objects!\n");
        return;
    }

    // 2. השמת הנתונים באובייקט החדש (האחרון במערך)
    map->objects[map->objectCount - 1] = InitMapObject(type, pos, scale);
}

GameMap *InitGameMap(GameMapID mapId)
{
    GameMap *map = malloc(sizeof(GameMap));
    map->mapId = mapId;

    switch (mapId)
    {
    case MAP_TUTORIAL:
        PopulateTutorialMap(map); // הקריאה לקובץ החדש שלך!
        break;
    case MAP_FOREST:
        // PopulateForestMap(map);
        break;
    default:
        break;
    }
    return map;
}

// מחזיר את מערך האובייקטים של המפה
MapObject *GetMapObjects(GameMap *map)
{
    if (map == NULL)
        return NULL;
    return map->objects;
}

// מחזיר את כמות האובייקטים שיש לצייר
int GetMapObjectCount(GameMap *map)
{
    if (map == NULL)
        return 0;
    return map->objectCount;
}

// מחזיר את אורך השלב
float GetMapLength(GameMap *map)
{
    if (map == NULL)
        return 0.0f;
    return map->mapLength;
}

// מחזיר את רוחב המסלול (Lane)
float GetMapLaneWidth(GameMap *map)
{
    if (map == NULL)
        return 0.0f;
    return map->laneWidth;
}

// מחזיר את סוג המודל של האובייקט
EnivormentResourcesTypes GetMapObjectType(MapObject *obj)
{
    return obj->modelType;
}

// מחזיר את המיקום של האובייקט בעולם
Vector3 GetMapObjectPosition(MapObject *obj)
{
    return obj->position;
}

// מחזיר את קנה המידה (Scale) של האובייקט
float GetMapObjectScale(MapObject *obj)
{
    return obj->scale;
}

void UnloadGameMap(GameMap *map)
{
    if (map == NULL)
        return;

    // 1. שחרור מערך האובייקטים (ה-MapObjects)
    if (map->objects != NULL)
    {
        free(map->objects);
        map->objects = NULL; // לביטחון, כדי שלא ננסה לגשת אליו שוב
    }

    // 2. שחרור מבנה המפה עצמו
    free(map);

    printf("Map memory cleared successfully.\n");
}

void UnloadMapAndLoadNext(GameState *gs, GameMapID nextLevelID)
{
    // שחרור המפה הישנה
    GameMap *gsMap = GetMap(gs);
    if (gsMap != NULL)
    {
        UnloadGameMap(gsMap);
        SetNewMap(gs, NULL); // איפוס זמני לביטחון
    }

    // טעינת המפה החדשה
    GameMap *nextMap = InitGameMap(nextLevelID);

    if (nextMap != NULL)
    {
        SetNewMap(gs, nextMap);
    }
    else
    {
        // טיפול בשגיאה - אולי לחזור לתפריט הראשי או להציג שגיאה
        printf("Error: Failed to load map %d\n", nextLevelID);
    }
}
