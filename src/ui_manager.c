#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#define RAYGUI_TEXTBOX_EXTENDED
#include "raygui.h"
#include "ui_manager.h"

static bool CheckIfPasswordValid(char *name, char *pass);

void EnterGameScreen(int screenWidth, int screenHeight, GameState *gameState)
{

    ClearBackground(RAYWHITE);
    float btnH = 60;  // הורדתי קצת את הגובה שיהיה פרופורציונלי
    float btnW = 400; // רוחב הגיוני יותר לטופס

    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    // נקודת התחלה
    Vector2 anchor = {(screenWidth / 2.0f) - (btnW / 2.0f), screenHeight / 6.0f};
    float padding = 20; // רווח קבוע בין אלמנטים

    // 1. כותרת
    GuiLabel((Rectangle){anchor.x, anchor.y, btnW, btnH}, "SUMMONER LOGIN");
    anchor.y += btnH + padding; // זזים למטה: גובה האלמנט + רווח

    // 2. תווית לשם
    GuiLabel((Rectangle){anchor.x, anchor.y, btnW, 30}, "Enter Name:");
    anchor.y += 35; // רווח קטן בין התווית לתיבה

    // 3. תיבת טקסט לשם
    static char nameBuffer[32] = "";
    static bool nameEditMode = false;
    static char passBuffer[32] = "";
    static bool passEditMode = false;

    // אם לוחצים על התיבה, היא הופכת לפעילה והשנייה לכבויה
    if (GuiTextBox((Rectangle){anchor.x, anchor.y, btnW, btnH}, nameBuffer, 32, nameEditMode))
    {
        nameEditMode = !nameEditMode;
        passEditMode = false; // מכבה את הסיסמה כשאנחנו בשם
    }

    anchor.y += btnH + padding;

    // 4. תווית לסיסמה
    GuiLabel((Rectangle){anchor.x, anchor.y, btnW, 30}, "Enter Password:");
    anchor.y += 35;

    // 5. תיבת טקסט לסיסמה
    if (GuiTextBox((Rectangle){anchor.x, anchor.y, btnW, btnH}, passBuffer, 32, passEditMode))
    {
        passEditMode = !passEditMode;
        nameEditMode = false; // מכבה את השם כשאנחנו בסיסמה
    }

    anchor.y += btnH + padding;

    // 6. כפתור התחברות
    if (GuiButton((Rectangle){anchor.x, anchor.y, btnW, btnH}, "LOG IN"))
    {
        if (strcmp(nameBuffer, "admin") == 0 && strcmp(passBuffer, "1234") == 0)
        {
            UpdateLoginState(gameState, SUB_LOGIN_CONNECTING);
            UpdateGameState(gameState , STATE_GAMEPLAY);
        }
        CheckIfPasswordValid(nameBuffer, passBuffer);
    }

    anchor.y += btnH + padding;

    if (GuiButton((Rectangle){anchor.x, anchor.y, btnW, btnH}, "Register"))
    {
        UpdateLoginState(gameState, SUB_LOGIN_REGISTERING);
        puts("update too SUB_LOGIN_REGISTERING");
    }
}

void RegisterScreen(int screenWidth, int screenHeight, GameState *gameState)
{
    ClearBackground(RAYWHITE);
    float btnH = 60;  // הורדתי קצת את הגובה שיהיה פרופורציונלי
    float btnW = 400; // רוחב הגיוני יותר לטופס

    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    // נקודת התחלה
    Vector2 anchor = {(screenWidth / 2.0f) - (btnW / 2.0f), screenHeight / 6.0f};
    float padding = 20; // רווח קבוע בין אלמנטים

    // 1. כותרת
    GuiLabel((Rectangle){anchor.x, anchor.y, btnW, btnH}, "SUMMONER Register Page");
    anchor.y += btnH + padding; // זזים למטה: גובה האלמנט + רווח

    // 2. תווית לשם
    GuiLabel((Rectangle){anchor.x, anchor.y, btnW, 30}, "Enter Name:");
    anchor.y += 35; // רווח קטן בין התווית לתיבה

    // 3. תיבת טקסט לשם
    static char nameBuffer[32] = "";
    static bool nameEditMode = false;
    static char passBuffer[32] = "";
    static bool passEditMode = false;
    static char passBufferCheck[32] = "";
    static bool passEditModeCheck = false;

    // אם לוחצים על התיבה, היא הופכת לפעילה והשנייה לכבויה
    if (GuiTextBox((Rectangle){anchor.x, anchor.y, btnW, btnH}, nameBuffer, 32, nameEditMode))
    {
        nameEditMode = !nameEditMode;
        passEditMode = false; // מכבה את הסיסמה כשאנחנו בשם
    }

    anchor.y += btnH + padding;

    // 4. תווית לסיסמה
    GuiLabel((Rectangle){anchor.x, anchor.y, btnW, 30}, "Enter Password:");
    anchor.y += 35;

    // 5. תיבת טקסט לסיסמה
    if (GuiTextBox((Rectangle){anchor.x, anchor.y, btnW, btnH}, passBuffer, 32, passEditMode))
    {
        passEditMode = !passEditMode;
        passEditModeCheck = false;
        nameEditMode = false; // מכבה את השם כשאנחנו בסיסמה
    }

    anchor.y += btnH + padding;

    GuiLabel((Rectangle){anchor.x, anchor.y, btnW, 30}, "Enter Password again for check:");
    anchor.y += 35;

    // 5. תיבת טקסט לסיסמה
    if (GuiTextBox((Rectangle){anchor.x, anchor.y, btnW, btnH}, passBufferCheck, 32, passEditModeCheck))
    {
        passEditModeCheck = !passEditModeCheck;
        passEditMode = false;
        nameEditMode = false; // מכבה את השם כשאנחנו בסיסמה
    }

    anchor.y += btnH + padding;

    if (GuiButton((Rectangle){anchor.x, anchor.y, btnW, btnH}, "REGISTER NOW"))
    {
        if (strlen(passBuffer) < 6)
        {
            // invalid pass message
        }
        if (strlen(nameBuffer) < 6)
        {
        }
        if (strcmp(passBuffer, passBufferCheck) != 0)
        {
            // invalid pass message
        }
        else
        {
            // need to check in database if name exists and add if valid
        }
    }
    anchor.y += btnH + padding;
    if (GuiButton((Rectangle){anchor.x, anchor.y, btnW, btnH}, "BACK"))
    {
        UpdateLoginState(gameState, SUB_LOGIN_ENTERING_NAME);
    }
}

static bool CheckIfPasswordValid(char *name, char *pass)
{
}
