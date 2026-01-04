#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#define RAYGUI_TEXTBOX_EXTENDED
#include "raygui.h"
#include "auth/auth_ui_manager.h"
#include "auth/auth.h"
#include "auth/db_auth_manager.h"

static const char *AuthStatusToMessage(AUTH_STATUS status);
static float DrawLabeledInput(Vector2 pos, float width, float height, const char *label, char *buffer, int bufferSize, bool *currentEditMode, bool *otherEdit1, bool *otherEdit2);

struct AuthUiState
{
    char name[32];
    char pass[32];
    char passConfirm[32];
    bool nameEdit;
    bool passEdit;
    bool confirmEdit;
    char errorMsg[128];
};

static float DrawLabeledInput(Vector2 pos, float width, float height, const char *label, char *buffer, int bufferSize, bool *currentEditMode, bool *otherEdit1, bool *otherEdit2)
{
    // נשלוף את גודל הפונט הנוכחי מהמערכת
    int fontSize = GuiGetStyle(DEFAULT, TEXT_SIZE);

    // גובה הלייבל חייב להיות לפחות כמו הפונט + קצת מרווח
    float labelHeight = fontSize + 10;

    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiLabel((Rectangle){pos.x, pos.y, width, labelHeight}, label);

    if (GuiTextBox((Rectangle){pos.x, pos.y + labelHeight, width, height}, buffer, bufferSize, *currentEditMode))
    {
        *currentEditMode = !(*currentEditMode);
        if (otherEdit1)
            *otherEdit1 = false;
        if (otherEdit2)
            *otherEdit2 = false;
    }

    return pos.y + labelHeight + height + 30;
}


void EnterGameScreen(int screenWidth, int screenHeight, GameState *gameState)
{
   
    static AuthUiState ui = {0}; // מאתחל הכל ל-0/false פעם אחת
    ClearBackground(RAYWHITE);
    float btnW = 400;
    float btnH = 60;
    int globalFontSize = 27;

    GuiSetStyle(DEFAULT, TEXT_SIZE, globalFontSize);

    Vector2 pos = {(screenWidth / 2.0f) - (btnW / 2.0f), screenHeight / 6.0f};

    // כותרת
    GuiLabel((Rectangle){pos.x, pos.y, btnW, btnH}, "SUMMONER LOGIN");
    pos.y += btnH + 25;

    // שדות קלט
    pos.y = DrawLabeledInput(pos, btnW, btnH, "Enter Name:", ui.name, 32, &ui.nameEdit, &ui.passEdit, NULL);
    pos.y = DrawLabeledInput(pos, btnW, btnH, "Enter Password:", ui.pass, 32, &ui.passEdit, &ui.nameEdit, NULL);

    if (ui.errorMsg[0] != '\0')
    {
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(RED));
        GuiLabel((Rectangle){pos.x, pos.y, btnW, 40}, ui.errorMsg);
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(DARKGRAY));
        pos.y += 45;
    }

    // כפתור התחברות
    if (GuiButton((Rectangle){pos.x, pos.y, btnW, btnH}, "LOG IN"))
    {
        if (strcmp(ui.name, "") == 0 && strcmp(ui.pass, "") == 0)
        {
            UpdateLoginState(gameState, SUB_LOGIN_CHOOSE_CHARACTER);
            UpdateGameState(gameState, STATE_LOGIN);
        }
        else if (ConnectToGame(ui.name, ui.pass, getDataBase(gameState), ui.errorMsg , gameState) == true)
        {
            UpdateLoginState(gameState, SUB_LOGIN_CHOOSE_CHARACTER);
            UpdateGameState(gameState, STATE_LOGIN);
            
        }
    }
    pos.y += btnH + 20;

    // כפתור מעבר להרשמה
    if (GuiButton((Rectangle){pos.x, pos.y, btnW, btnH}, "Register"))
    {
        UpdateLoginState(gameState, SUB_LOGIN_REGISTERING);
    }
}

void RegisterScreen(int screenWidth, int screenHeight, GameState *gameState)
{
   

    static AuthUiState ui = {0};
    ClearBackground(RAYWHITE);

    float btnW = 400;
    float btnH = 60;
    int globalFontSize = 27;

    GuiSetStyle(DEFAULT, TEXT_SIZE, globalFontSize);
    Vector2 pos = {(screenWidth / 2.0f) - (btnW / 2.0f), screenHeight / 10.0f};

    // כותרת
    GuiLabel((Rectangle){pos.x, pos.y, btnW, btnH}, "SUMMONER REGISTER");
    pos.y += btnH + 40;

    // שדות קלט - שימוש חוזר בפונקציית העזר
    pos.y = DrawLabeledInput(pos, btnW, btnH, "Name:", ui.name, 32, &ui.nameEdit, &ui.passEdit, &ui.confirmEdit);
    pos.y = DrawLabeledInput(pos, btnW, btnH, "Password:", ui.pass, 32, &ui.passEdit, &ui.nameEdit, &ui.confirmEdit);
    pos.y = DrawLabeledInput(pos, btnW, btnH, "Confirm Password:", ui.passConfirm, 32, &ui.confirmEdit, &ui.nameEdit, &ui.passEdit);

    // הצגת שגיאה (אם קיימת)
    if (ui.errorMsg[0] != '\0')
    {
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(RED));
        GuiLabel((Rectangle){pos.x, pos.y, btnW, 40}, ui.errorMsg);
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(DARKGRAY));
        pos.y += 55;
    }

    if (GuiButton((Rectangle){pos.x, pos.y, btnW, btnH}, "REGISTER NOW"))
    {
        AUTH_STATUS status;
        if (CheckIfUserExists(ui.name, getDataBase(gameState) , ui.errorMsg) == true)
        {
            status = USER_ALREADY_EXISTS;
        }
        else
        {
            status = HandleRegisterRules(ui.name, ui.pass, ui.passConfirm);
            if (status == AUTH_SUCCESS)
            {
                ui.errorMsg[0] = '\0';
            }
            else
            {
                strncpy(ui.errorMsg, AuthStatusToMessage(status), 127);
            }
        }

        if (status == AUTH_SUCCESS)
        {
            if (AddUserToDatabase(ui.name, ui.pass, getDataBase(gameState), ui.errorMsg) == true)
            {
                memset(&ui, 0, sizeof(AuthUiState));
                UpdateLoginState(gameState, SUB_LOGIN_ENTERING_NAME);
            }
        }
    }
    pos.y += btnH + 30;
    if (GuiButton((Rectangle){pos.x, pos.y, btnW, btnH}, "BACK"))
    {
        memset(&ui, 0, sizeof(AuthUiState));
        UpdateLoginState(gameState, SUB_LOGIN_ENTERING_NAME);
    }
}

static const char *AuthStatusToMessage(AUTH_STATUS status)
{
    switch (status)
    {
    case AUTH_SUCCESS:
        return "";
    case NAME_EMPTY:
        return "Error: Name cannot be empty!";
    case NAME_TOO_SHORT:
        return "Error: Name is too short!";
    case NAME_TOO_LONG:
        return "Error: Name is too long!";
    case NAME_INVALID_COMPLEXITY:
        return "Error: Name contains invalid characters!";
    case PASS_EMPTY:
        return "Error: Password cannot be empty!";
    case PASS_TOO_SHORT:
        return "Error: Password too short!";
    case PASS_TOO_LONG:
        return "Error: Password too long!";
    case PASS_MISMATCH:
        return "Error: Passwords do not match!";
    case PASS_INVALID_COMPLEXITY:
        return "Error: Password is too weak!";
    default:
        return "Error: Unknown authentication error.";
    }
}
