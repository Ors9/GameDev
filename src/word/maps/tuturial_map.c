#include "word/map_manager.h"
#include <raylib.h>

void PopulateTutorialMap(GameMap *map)
{
    // כאן אנחנו מגדירים את נתוני השלב הספציפי
    int count = 50;

    // הקצאת המערך (מכיוון שזה קובץ C אחר, נשתמש ב-Getters/Setters אם צריך,
    // אבל בדרך כלל נחשוף את ה-Struct ל-MapManager בלבד)

    // דוגמה למילוי אובייקטים (הלוגיקה שהייתה לך ב-InitGameMap)
    for (int i = 0; i < count; i++)
    {
        Vector3 pos = {i * 15.0f, 220.0f, (float)GetRandomValue(-5, 5)};
        float scale = (float)GetRandomValue(2, 5);

        // הוספת האובייקט למפה (נשתמש בפונקציה שנבנה ב-MapManager)
        AddObjectToMap(map, ENIV_WORD_TERRIAN, pos, scale);
    }
}