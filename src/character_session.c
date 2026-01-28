#include <game_types.h>
#include <character_session.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <characters.h>
#include "game_state.h"

struct CharacterSession
{
    int cid;
    char cname[32];
    Player *player;
    CharacterClass class_type;
    int level;
    int xp;
};

void DrawEntityLabel(GameState *gs)
{
    GameCamera *gCam = GetMainCamera(gs);
    Camera3D raylibCam = GetRaylibCamera(gCam);

    UserSession *session = GetUserSession(gs);
    CharacterSession *cs = GetCharacterSession(session);
    Player *player = GetPlayer(cs);
    Vector3 position = GetPlayerPosition(player);
    CharacterStats *charStats = GetCharacterStats(player);
    int health = GetCurrentHealth(player);
    int maxHealth = GetMaxHealth(player);

    // 1. חישוב מיקום מעל הראש (גובה 2.5 יחידות - תשנה לפי המודל שלך)
    Vector3 topPos = {position.x, position.y + 5.0f, position.z};

    // 2. המרה לדו-מימד
    Vector2 screenPos = GetWorldToScreen(topPos, raylibCam);

    // בדיקה בסיסית: אם screenPos היא (0,0), ייתכן והאובייקט מחוץ לפריים או מאחורי המצלמה
    if (screenPos.x <= 0 || screenPos.y <= 0)
        return;

    // --- עיצוב הטקסט ---
    int fontSize = 20;
    const char *labelText = TextFormat("%s [Lv %d]", cs->cname, cs->level);
    int textWidth = MeasureText(labelText, fontSize);

    // ציור טקסט (מרכוז לפי הרוחב)
    DrawText(labelText, screenPos.x - (textWidth / 2), screenPos.y - 40, fontSize, BLACK);

    // --- עיצוב בר החיים (Health Bar) ---
    float barWidth = 80.0f;
    float barHeight = 8.0f;
    float healthPercent = (float)health / maxHealth;

    // מיקום הבר מתחת לטקסט
    float barX = screenPos.x - (barWidth / 2);
    float barY = screenPos.y - 15;

    // רקע אדום (מה שחסר)
    DrawRectangle(barX, barY, barWidth, barHeight, MAROON);
    // בר ירוק (מה שיש)
    DrawRectangle(barX, barY, barWidth * healthPercent, barHeight, LIME);
    // מסגרת שחורה דקה
    DrawRectangleLinesEx((Rectangle){barX, barY, barWidth, barHeight}, 1, BLACK);
    int screenWidth = GetScreenWidth();


    int screenHeight = GetScreenHeight();



    // חישוב המיקום: גובה המסך פחות עובי הבר
    int yPosition = screenHeight - barHeight;

    // ציור רקע לבר (כדי שיראו כמה חסר - למשל בצבע אפור כהה)
    DrawRectangle(0, yPosition, screenWidth, barHeight, DARKGRAY);

    // חישוב רוחב ה-EXP לפי התקדמות (למשל 60% מהרמה)
    float expPercentage = cs->xp / 100; // כאן תכניס את החישוב שלך (current_exp / max_exp)
    int expWidth = (int)(screenWidth * expPercentage);

    // ציור ה-EXP עצמו (למשל בצבע סגול או תכלת)
    DrawRectangle(0, yPosition, expWidth, barHeight, PURPLE);
}

Player *GetPlayer(CharacterSession *cs)
{
    return cs->player;
}

void PrintCharacterSession(CharacterSession *session)
{
    if (session == NULL)
    {
        printf("[CharacterSession] Error: Attempted to print a NULL session.\n");
        return;
    }

    printf("--- Character Session Info ---\n");
    printf("ID:      %d\n", session->cid);
    printf("Name:    %s\n", session->cname);
    printf("Class:   %d\n", (int)session->class_type); // Cast to int for safety
    printf("Level:   %d\n", session->level);
    printf("XP:      %d\n", session->xp);

    if (session->player != NULL)
    {
        printf("Status:  Active (Player object initialized)\n");
    }
    else
    {
        printf("Status:  Inactive (No Player object)\n");
    }
    printf("------------------------------\n");
}

CharacterSession *InitCharacterSession(int cid, char *cname, CharacterClass class_type, int level, int xp)
{
    CharacterSession *session = malloc(sizeof(CharacterSession));
    if (session == NULL)
    {
        printf("Failed to allocate memory for CharacterSession.\n");
        exit(1);
    }
    session->cid = cid;
    session->class_type = class_type;
    session->level = level;
    session->xp = xp;
    strncpy(session->cname, cname, sizeof(session->cname) - 1);
    session->cname[sizeof(session->cname) - 1] = '\0';
    session->player = NULL;
    return session;
}

CharacterClass GetCharacterClass(CharacterSession *session)
{
    return session->class_type;
}

void UpdateCharacterSession(CharacterSession *session, char *cname, int cid, CharacterClass class_type, int level, int xp)
{
    session->cid = cid;
    session->class_type = class_type;
    session->level = level;
    session->xp = xp;
    strncpy(session->cname, cname, sizeof(session->cname) - 1);
    session->cname[sizeof(session->cname) - 1] = '\0';
}

CharacterSession *CreateEmptyCharacterList(int size)
{

    CharacterSession *chars = malloc(sizeof(CharacterSession) * size);
    if (chars == NULL)
    {
        perror("Failed to allocate CharacterSession list");
        return NULL;
    }

    for (int i = 0; i < size; i++)
    {
        chars[i].cid = -1;
        chars[i].class_type = -1;
        chars[i].level = 0;
        chars[i].xp = 0;
        chars[i].cname[0] = '\0';
    }
    return chars;
}

void InitCharacterPlayer(CharacterSession *session, AssetManager *assets)
{
    session->player = InitPlayer(session->class_type, assets);
    PrintCharacterSession(session);
}

CharacterSession *GetCharacterFromList(CharacterSession *list, int index)
{
    return &list[index];
}

void UnloadCharacterSession(CharacterSession *session)
{

    if (session != NULL)
    {
        free(session);
        session = NULL;
        printf("CharacterSession memory cleared successfully.\n");
    }
}

char *GetCharacterName(CharacterSession *session)
{
    return session->cname;
}

int GetCharacterLevel(CharacterSession *session)
{
    return session->level;
}

// תוסיף את זה ל-src/character_session.c
CharacterSession *CloneCharacterSession(CharacterSession *source)
{
    if (source == NULL)
        return NULL;

    // יוצר רשימה חדשה של איבר אחד
    CharacterSession *newNode = CreateEmptyCharacterList(1);

    // מעתיק את כל הנתונים מהמקור
    UpdateCharacterSession(
        newNode,
        GetCharacterName(source),
        GetCharacterId(source),
        GetCharacterClass(source),
        GetCharacterLevel(source),
        GetCharacterXP(source));

    return newNode;
}

int GetCharacterId(CharacterSession *session)
{
    if (session == NULL)
        return -1;
    return session->cid; // וודא שזה השם של השדה אצלך ב-struct (כנראה cid לפי ה-SQL)
}

int GetCharacterXP(CharacterSession *session)
{
    if (session == NULL)
        return 0;
    return session->xp; // וודא שזה השם של השדה אצלך
}
