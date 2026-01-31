#include "word/map_manager.h"

void PopulateTutorialMap(GameMap *map)
{
    // מיקום קבוע: X=15 (קצת לפני השחקן), Y=220 (גובה הרצפה), Z=0 (מרכז המסלול)
    Vector3 pos = {15.0f, 220.0f, 0.0f};

    // גודל קבוע: 5.0 נחשב לגודל טוב לסלע שרואים בבירור
    float scale = 5.0f;

    AddObjectToMap(map, ENIV_WORD_TERRIAN, pos, scale);
}