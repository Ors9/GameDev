#include <raylib.h>
#include "word/map_manager.h"
#include "assets/assets_manager.h"
#include "stdlib.h"
#include <stdio.h>
#include <game_state.h>
#include "game_types.h"
#include "word/maps/tuturial_map.h"
#include "rlgl.h"

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

    if (map == NULL)
    {
        printf("InitGameMap malloc fail\n");
        exit(1);
    }
    map->objects = NULL;
    map->mapId = mapId;
    map->objectCount = 0;
    map->mapLength = 2000.0f;
    map->laneWidth = 40.0f;

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

void DrawMap(GameMap *map, AssetManager *assets)
{
    if (map == NULL || assets == NULL)
        return;

    rlDisableBackfaceCulling();
    rlEnableDepthTest();

    for (int i = 0; i < map->objectCount; i++)
    {
        MapObject *obj = &map->objects[i];

        if (IsEnvResourceReady(assets, obj->modelType))
        {
            Model model = GetEnvModelByType(assets, obj->modelType);

            // ציור המופע הספציפי מהמפה
            DrawModel(model, obj->position, obj->scale, WHITE);
        }
    }

    rlEnableBackfaceCulling();
}

float GetMapHeightAt(GameMap *map, AssetManager *assets, Vector3 position)
{
    if (map == NULL || assets == NULL)
        return 220.0f; // גובה רצפה בסיסי כברירת מחדל

    float highestHitY = 220.0f; // מתחילים מגובה הרצפה
    bool hitFound = false;

    // יוצרים קרן שמתחילה מעל הראש של השחקן (למשל 10 יחידות למעלה) ויורה למטה

    Ray ray = {
        (Vector3){position.x, position.y + 50.0f, position.z},
        (Vector3){0, -1, 0}};
    /*

            Ray ray = {
    (Vector3){position.x, position.y + 10.0f, position.z},
    (Vector3){0, -1, 0}};

        Ray ray = {
        (Vector3){position.x, position.y + 50.0f, position.z}, // להתחיל גבוה יותר
        (Vector3){0, -1, 0}};

    */

    // עוברים על כל האובייקטים במפה
    for (int i = 0; i < map->objectCount; i++)
    {
        MapObject *obj = &map->objects[i];

        if (IsEnvResourceReady(assets, obj->modelType))
        {
            Model model = GetEnvModelByType(assets, obj->modelType);

            // בונים את המטריצה של האובייקט הספציפי הזה מהמפה
            Matrix transform = MatrixMultiply(
                MatrixScale(obj->scale, obj->scale, obj->scale),
                MatrixTranslate(obj->position.x, obj->position.y, obj->position.z));

            // בודקים התנגשות מול כל ה-Meshes של המודל
            for (int m = 0; m < model.meshCount; m++)
            {
                RayCollision hit = GetRayCollisionMesh(ray, model.meshes[m], transform);

                // אם פגענו במשהו והוא גבוה יותר מהגובה שמצאנו עד עכשיו
                if (hit.hit && hit.point.y > highestHitY)
                {
                    highestHitY = hit.point.y;
                    hitFound = true;
                }
            }
        }
    }

    return highestHitY;
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

void PrintMapInfo(GameMap *map)
{
    if (map == NULL)
    {
        printf("[MAP DEBUG] Map is NULL!\n");
        return;
    }

    printf("\n--- [MAP DEBUG INFO] ---\n");
    printf("Map ID: %d\n", map->mapId);
    printf("Total Objects: %d\n", map->objectCount);
    printf("Map Dimensions: Length %.2f, Lane Width %.2f\n", map->mapLength, map->laneWidth);

    if (map->objects == NULL && map->objectCount > 0)
    {
        printf("WARNING: Object count is %d but objects pointer is NULL!\n", map->objectCount);
        return;
    }

    printf("Objects List:\n");
    for (int i = 0; i < map->objectCount; i++)
    {
        MapObject obj = map->objects[i];
        printf("  [%d] Type: %d | Pos: (%.1f, %.1f, %.1f) | Scale: %.1f\n",
               i,
               obj.modelType,
               obj.position.x, obj.position.y, obj.position.z,
               obj.scale);
    }
    printf("------------------------\n\n");
}