#include "word/map_manager.h"

void PopulateTutorialMap(GameMap *map)
{
    float tileScale = 20.0f;
    float spacing = 20.0f;
    int length = 100;
    int width = 3;

    // 1. יצירת הרצפה
    for (int i = 0; i < length; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            Vector3 pos = {i * spacing, 220.0f, j * spacing};
            AddObjectToMap(map, ENIV_FLOOR_BLOCK, pos, tileScale);
        }
    }

    // 2. הוספת אובייקט מעל הרצפה
    // שים לב לשינוי: Y = 220.1f (כדי למנוע Z-Fighting)
    Vector3 obstaclePos = {100.0f, 220.1f, 0.0f};
    AddObjectToMap(map, ENIV_FLOOR_BLOCK, obstaclePos, 5.0f);
}