#include "raygui.h"
#include "game_state.h"
#include <characters.h>
#include <string.h>
#include <assets_manager.h>
#include <ctype.h>
#include <user_session.h>
#include <character_session.h>
#include <choose_character.h>
#include <stdlib.h>

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#define RAYGUI_TEXTBOX_EXTENDED

typedef struct CharacterSession CharacterSession;

static float DrawLblInput(Vector2 pos, float width, float height, const char *label, char *buffer, int bufferSize, bool *currentEditMode);
static void DrawCharacters(GameState *gs, CharacterClass selectedClass);
static bool IsValidName(const char *name, char *errorText);
static bool AddCharClassToDB(char *name, CharacterClass cls, GameState *gs, char *errorText);
static void DrawLoginCharacters(GameState *gs, int selectedCharacter, int characterCount);
static int LoadUserCharacters(GameState *gs);

// פונקציית עזר לטעינת דמויות מה-DB לתוך ה-GameState
// עדכון פונקציית הטעינה כדי שתכניס נתונים לתוך ה-Session
static int LoadUserCharacters(GameState *gs)
{
    PGconn *db = getDataBase(gs);
    UserSession *session = GetUserSession(gs);
    int userId = GetUserIdFromSession(session);

    char userIdStr[12];
    snprintf(userIdStr, sizeof(userIdStr), "%d", userId);
    const char *query = "SELECT cid, user_id, class_type, cname, level, xp FROM UserCharacters WHERE user_id = $1";
    const char *params[1] = {userIdStr};

    PGresult *res = PQexecParams(db, query, 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        PQclear(res);
        return 0;
    }

    int rows = PQntuples(res);
    CharacterSession *list = CreateEmptyCharacterList(rows);

    for (int i = 0; i < rows; i++)
    {
        UpdateCharacterSession(
            GetCharacterFromList(list, i),
            PQgetvalue(res, i, 3),
            atoi(PQgetvalue(res, i, 0)),
            (CharacterClass)atoi(PQgetvalue(res, i, 2)),
            atoi(PQgetvalue(res, i, 4)),
            atoi(PQgetvalue(res, i, 5)));
    }

    // שומרים את כל הרשימה בתוך ה-session
    SetUserCharacterList(session, list, rows);

    PQclear(res);
    return rows;
}


void ChoosePlayerScreen(int screenWidth, int screenHeight, GameState *gs)
{
    ClearBackground(RAYWHITE);
    static int selectedCharacter = 0;
    static bool dataLoaded = false;

    // 1. משיגים את הסשן
    UserSession *session = GetUserSession(gs);

    // 2. טעינה חד-פעמית
    if (!dataLoaded)
    {
        LoadUserCharacters(gs);
        dataLoaded = true;
    }

    // 3. שליפת נתונים מהסשן לציור
    CharacterSession *list = GetSessionCharacterList(session);
    int characterCount = GetSessionCharacterCount(session);

    // ציור המודלים ב-3D
    DrawLoginCharacters(gs, selectedCharacter, characterCount);

    // 4. כותרת המסך
    GuiSetStyle(DEFAULT, TEXT_SIZE, 35);
    GuiLabel((Rectangle){(screenWidth / 2.0f) - 200, 50, 400, 60}, "Choose Your Character");

    // 5. ציור כפתורים דינמי לבחירת דמות
    for (int i = 0; i < characterCount; i++)
    {
        Rectangle btnRec = {100 + (i * 200), screenHeight - 220, 150, 40};
        CharacterSession *cur = GetCharacterFromList(list, i);
        
        if (GuiButton(btnRec, GetCharacterName(cur)))
        {
            selectedCharacter = i;
        }

        if (selectedCharacter == i)
        {
            DrawRectangleLinesEx(btnRec, 3, RED);
        }
    }

    // --- כאן החזרנו את הלוגיקה והכפתורים שנעלמו ---

    // 6. כפתור START GAME - עם לוגיקת הניקוי
    if (GuiButton((Rectangle){(screenWidth / 2.0f) - 200, screenHeight - 140, 400, 60}, "START GAME"))
    {
        // מבצעים את הבחירה הסופית (העתקת הדמות וניקוי הרשימה)
        FinalizeCharacterSelection(session, selectedCharacter , getAssetManager(gs));

        // הדפסת דיבאג (הכמימוס שביקשת)
        PrintUserSessionDebug(session);

        // מעבר למשחק עצמו
        UpdateGameState(gs, STATE_GAMEPLAY);
    }

    // 7. כפתור CREATE NEW CHARACTER (בתחתית המסך)
    if (GuiButton((Rectangle){(screenWidth / 2.0f) - 200, screenHeight - 70, 400, 60}, "+ Create New Character"))
    {
        dataLoaded = false; // כדי שיטען מחדש כשנחזור
        UpdateLoginState(gs, SUB_LOGIN_CREATE_CHARACTER);
    }
}
void CreateCharacterScreen(int screenWidth, int screenHeight, GameState *gs)
{
    ClearBackground(RAYWHITE);

    // 1. נתונים סטטיים (עדיף להעביר ל-gs בהמשך)
    static CharacterClass selectedClass = MUTANT_CHAR;
    static char charName[20] = "\0";
    static char errorText[100] = "\0";
    static bool nameEditMode = false;

    float btnW = 400;
    float btnH = 60;
    int globalFontSize = 35;
    GuiSetStyle(DEFAULT, TEXT_SIZE, globalFontSize);

    Vector2 pos = {(screenWidth / 2.0f) - (btnW / 2.0f), 50};

    DrawCharacters(gs, selectedClass);

    GuiLabel((Rectangle){pos.x, pos.y, btnW, btnH}, "Create Player Screen");
    pos.y += btnH + 20;

    // בחירת קלאס - כפתורים בצדדים
    if (GuiButton((Rectangle){100, screenHeight / 2.0f, 200, 50}, "MUTANT"))
    {
        selectedClass = MUTANT_CHAR;
    }

    if (GuiButton((Rectangle){screenWidth - 300, screenHeight / 2.0f, 200, 50}, "MONSTER"))
    {
        selectedClass = MONSTER_CHAR;
    }

    Vector2 inputPos = {(screenWidth / 2.0f) - 150, screenHeight - 250};
    DrawLblInput(inputPos, 300, 50, "Character Name:", charName, 20, &nameEditMode);

    if (GuiButton((Rectangle){(screenWidth / 2.0f) - 250, screenHeight - 150, 500, 60}, "Create Chosen Character"))
    {
        if (IsValidName(charName, errorText) == true)
        {

            if (AddCharClassToDB(charName, selectedClass, gs, errorText) == true)
            {
                UpdateLoginState(gs, SUB_LOGIN_CHOOSE_CHARACTER);
                errorText[0] = '\0';
            }
        }
    }

    if (GuiButton((Rectangle){(screenWidth / 2.0f) - 250, screenHeight - 70, 500, 60}, "Back"))
    {
        UpdateLoginState(gs, SUB_LOGIN_CHOOSE_CHARACTER);
    }

    // ציור השגיאה (מחוץ ל-if של הכפתור) - ירוץ בכל פריים אם יש טקסט
    if (errorText[0] != '\0')
    {
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(RED)); // צבע אדום לשגיאה
        GuiLabel((Rectangle){(screenWidth / 2.0f) - 250, screenHeight - 20, 500, 30}, errorText);
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(GRAY)); // החזרה לצבע רגיל
    }
}

static bool AddCharClassToDB(char *name, CharacterClass cls, GameState *gs, char *errorText)
{

    PGconn *dataBase = getDataBase(gs);
    if (dataBase == NULL)
    {
        strncpy(errorText, "Database connection lost.", 100);
        return false;
    }

    const char *query = "INSERT INTO UserCharacters (user_id, class_type, cname) VALUES ($1, $2, $3)";

    // המרה למחרוזות עבור PQexecParams
    char userIdStr[12];
    char classStr[12];
    UserSession *session = GetUserSession(gs);
    snprintf(userIdStr, sizeof(userIdStr), "%d", GetUserIdFromSession(session));
    snprintf(classStr, sizeof(classStr), "%d", (int)cls);

    const char *paramValues[3];
    paramValues[0] = userIdStr;
    paramValues[1] = classStr;
    paramValues[2] = name;

    PGresult *res = PQexecParams(dataBase, query, 3, NULL, paramValues, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {

        snprintf(errorText, 100, "DB Error: %s", PQresultErrorMessage(res));
        printf("DB Error: %s\n", PQresultErrorMessage(res));

        PQclear(res);
        return false;
    }

    PQclear(res);

    if (errorText)
        errorText[0] = '\0';

    return true;
}

static bool IsValidName(const char *name, char *errorText)
{
    if (name == NULL)
    {
        strcpy(errorText, "Name must be between 4 and 10 characters.");
        return false;
    }

    size_t len = strlen(name);
    if (len < 4 || len > 10)
    {
        strcpy(errorText, "Name must be between 4 and 10 characters.");
        return false;
    }

    bool foundNumber = false;
    bool foundCharacter = false;

    for (int i = 0; name[i] != '\0'; i++)
    {
        if (isdigit(name[i]))
        {
            foundNumber = true;
        }
        else if (isalpha(name[i]))
        {
            foundCharacter = true;
        }
        else
        {
            strcpy(errorText, "Name must contain only letters and numbers.");
            return false;
        }
    }

    if (!foundCharacter)
    {
        strcpy(errorText, "Name must contain at least one letter.");
        return false;
    }

    return foundCharacter;
}

static void DrawLoginCharacters(GameState *gs, int selectedCharacter, int characterCount)
{
    GameCamera *mainCamera = GetMainCamera(gs);
    AssetManager *assets = getAssetManager(gs);
    MyUpdateCamera(mainCamera, (Vector3){10.0f, 0.0f, 0.0f}, GetFrameTime());
    Camera3D camera = GetRaylibCamera(mainCamera);

    CharacterSession *list = GetSessionCharacterList(GetUserSession(gs));

    BeginMode3D(camera);
    for (int i = 0; i < characterCount; i++)
    {
        CharacterSession *current = GetCharacterFromList(list, i);
        Vector3 pos = {5.0f + (i * 10.0f), 0.0f, 0.0f};

        // ציור המודל
        DrawModel(GetModel(GetCharacterRescource(assets, GetCharacterClass(current))), pos, 3.0f, WHITE);

        // ציור סימון בחירה מתחת לדמות
        if (i == selectedCharacter)
        {
            DrawCircle3D(pos, 5.0f, (Vector3){1, 0, 0}, 90.0f, RED);
        }
    }
    EndMode3D();
}

static void DrawCharacters(GameState *gs, CharacterClass selectedClass)
{
    GameCamera *mainCamera = GetMainCamera(gs);
    AssetManager *assets = getAssetManager(gs);
    MyUpdateCamera(mainCamera, (Vector3){10.0f, 0.0f, 0.0f}, GetFrameTime());
    Camera3D camera = GetRaylibCamera(mainCamera);
    CharacterResources *res1 = GetCharacterRescource(assets, MUTANT_CHAR);
    CharacterResources *res2 = GetCharacterRescource(assets, MONSTER_CHAR);
    float deltaTime = GetFrameTime();
    BeginMode3D(camera);

    DrawModel(GetModel(res1), (Vector3){5.0f, 0.0f, 0.0f}, 3.0f, WHITE);

    DrawModel(GetModel(res2), (Vector3){15.0f, 0.0f, 0.0f}, 3.0f, WHITE);

    if (selectedClass == MUTANT_CHAR)
    {
        DrawCircle3D((Vector3){5.0f, 0.0f, 0.0f}, 5.0f, (Vector3){5.0f, 0.0f, 0.0f}, 10.0f, RED);
    }
    else if (selectedClass == MONSTER_CHAR)
    {
        DrawCircle3D((Vector3){15.0f, 0.0f, 0.0f}, 5.0f, (Vector3){5.0f, 0.0f, 0.0f}, 10.0f, RED);
    }

    EndMode3D();
}

static float DrawLblInput(Vector2 pos, float width, float height, const char *label, char *buffer, int bufferSize, bool *currentEditMode)
{
    int fontSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    float labelHeight = fontSize + 10;

    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiLabel((Rectangle){pos.x, pos.y, width, labelHeight}, label);

    // התיקון כאן: הוספת שינוי המצב בתוך ה-if
    if (GuiTextBox((Rectangle){pos.x, pos.y + labelHeight, width, height}, buffer, bufferSize, *currentEditMode))
    {
        *currentEditMode = !(*currentEditMode); // הופך את המצב מ-false ל-true ולהפך
    }

    return pos.y + labelHeight + height + 30;
}