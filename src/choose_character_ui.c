#include "raygui.h"
#include "game_state.h"
#include <characters.h>
#include <string.h>
#include <assets_manager.h>

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#define RAYGUI_TEXTBOX_EXTENDED

static float DrawLblInput(Vector2 pos, float width, float height, const char *label, char *buffer, int bufferSize, bool *currentEditMode);
static void DrawCharacters(GameState *gs);


void ChoosePlayerScreen(int screenWidth, int screenHeight, GameState *gs)
{
    ClearBackground(RAYWHITE);
    float btnW = 400;
    float btnH = 60;
    int globalFontSize = 35;

    GuiSetStyle(DEFAULT, TEXT_SIZE, globalFontSize);

    Vector2 pos = {(screenWidth / 2.0f) - (btnW / 2.0f), screenHeight / 6.0f};

    // כותרת
    GuiLabel((Rectangle){pos.x, pos.y, btnW, btnH}, "Choose Player screen");

    pos.y += btnH + 25;

    if (GuiButton((Rectangle){pos.x, pos.y, btnW, btnH}, "Create character"))
    {
        UpdateLoginState(gs, SUB_LOGIN_CREATE_CHARACTER);
    }
}

void CreateCharacterScreen(int screenWidth, int screenHeight, GameState *gs)
{
    ClearBackground(RAYWHITE);
    float btnW = 400;
    float btnH = 60;
    int globalFontSize = 35;

    GuiSetStyle(DEFAULT, TEXT_SIZE, globalFontSize);

    Vector2 pos = {(screenWidth / 2.0f) - (btnW / 2.0f), screenHeight / 6.0f};

    // כותרת
    GuiLabel((Rectangle){pos.x, pos.y, btnW, btnH}, "Create Player screen");

    DrawCharacters(gs);

    pos.y += btnH + 500;
    if (GuiButton((Rectangle){pos.x, pos.y, btnW, btnH}, "Back to choose player screen"))
    {
        UpdateLoginState(gs, SUB_LOGIN_CHOOSE_CHARACTER);
    }
}

static void DrawCharacters(GameState *gs)
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

    EndMode3D();
}

static float DrawLblInput(Vector2 pos, float width, float height, const char *label, char *buffer, int bufferSize, bool *currentEditMode)
{
    // נשלוף את גודל הפונט הנוכחי מהמערכת
    int fontSize = GuiGetStyle(DEFAULT, TEXT_SIZE);

    // גובה הלייבל חייב להיות לפחות כמו הפונט + קצת מרווח
    float labelHeight = fontSize + 10;

    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiLabel((Rectangle){pos.x, pos.y, width, labelHeight}, label);

    if (GuiTextBox((Rectangle){pos.x, pos.y + labelHeight, width, height}, buffer, bufferSize, *currentEditMode))
    {
    }

    return pos.y + labelHeight + height + 30;
}