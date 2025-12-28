#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "raylib.h"
#include "raymath.h"


// הגדרת מצבי המשחק השונים
typedef enum MainGameScreenState
{
    STATE_LOGIN,    // מסך התחברות / הרשמה
    STATE_GAMEPLAY, // המשחק עצמו
    STATE_EXIT      // יציאה וניקוי זיכרון
} MainGameScreenState;

typedef enum SubStateLogin
{
    SUB_LOGIN_NONE = 0,
    SUB_LOGIN_WAITING,
    SUB_LOGIN_REGISTERING,
    SUB_LOGIN_ENTERING_NAME,
    SUB_LOGIN_CONNECTING
} SubStateLogin;

typedef enum SubStateGameplay
{
    SUB_GAMEPLAY_NONE = 0,
    SUB_GAMEPLAY_PLAYING,
    SUB_GAMEPLAY_PAUSED,
    SUB_GAMEPLAY_WAITING,
} SubStateGameplay;

typedef enum SubStateExit
{
    SUB_EXIT_NONE = 0,
    SUB_EXIT_WAITING,
    SUB_EXIT_CLEAN_MEMORY,
    SUB_EXIT_EXITING,
    
} SubStateExit;

typedef struct GameState GameState;


GameState * InitGameState();
GameState * UnloadGameState(GameState * gs);
SubStateLogin   getLoginState(GameState *gs);

void UpdateGameState(GameState *gs , MainGameScreenState newState);
void UpdateLoginState(GameState *gs , SubStateLogin newState);
void UpdateGameplayState(GameState *gs , SubStateGameplay newState);
void UpdateExitState(GameState *gs , SubStateExit newState);
void HandleCurrentScreenState(GameState *gs);



#endif // GAME_STATE_H