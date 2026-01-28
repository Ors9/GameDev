#include "game_types.h"
#include "user_session.h"

#ifndef CHARACTERS_H
#define CHARACTERS_H

typedef struct CharacterSession CharacterSession;
typedef struct AssetManager AssetManager;
typedef struct Player Player;
typedef struct GameState GameState;

void UnloadCharacterSession(CharacterSession *session);
Player* GetPlayer(CharacterSession *cs);
CharacterSession* CloneCharacterSession(CharacterSession* source);
void UpdateCharacterSession(CharacterSession *session, char *cname, int cid, CharacterClass class_type, int level, int xp);
CharacterSession *InitCharacterSession(int cid,char *cname , CharacterClass class_type, int level, int xp);
CharacterSession *CreateEmptyCharacterList(int size);
CharacterSession* GetCharacterFromList(CharacterSession* list, int index);
CharacterClass GetCharacterClass(CharacterSession *session);
char *GetCharacterName(CharacterSession *session);
int GetCharacterLevel(CharacterSession *session);
int GetCharacterId(CharacterSession *session);
void InitCharacterPlayer(CharacterSession * session , AssetManager *assets);
void FinalizeCharacterSelection(UserSession *session, int selectedIndex , AssetManager *assets);
int GetCharacterXP(CharacterSession *session);
void DrawEntityLabel(GameState *gs);
#endif // CHARACTERS_H