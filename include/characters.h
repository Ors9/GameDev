#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "raymath.h"
#include "game_types.h"



typedef struct AssetManager AssetManager; // Struct of asset_manager.h
typedef struct CharacterStats CharacterStats;
typedef struct Player Player;
typedef struct GameState GameState;// Struct on game_state.h


// הצהרה על הפונקציות (בלי המימוש שלהן)
Player *InitPlayer(CharacterClass selectedClass, AssetManager *asset);
bool MovingPlayer(Player *player, float deltaTime);
void DrawPlayer(Player * player);
void UpdatePlayer(Player *player, float deltaTime , GameState *gs);
bool AnimationController(Player *player);
Vector3 GetPlayerPosition(Player *player);
void UnloadPlayer(Player * player);
int GetCurrentHealth(Player * p);
CharacterStats * GetCharacterStats(Player * p);
int GetMaxHealth(Player *p);

#endif // PLAYER_H
