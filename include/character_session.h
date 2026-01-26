#include "game_types.h"
#include "user_session.h"

#ifndef CHARACTERS_H
#define CHARACTERS_H

typedef struct CharacterSession CharacterSession;
typedef struct AssetManager AssetManager;

void UnloadCharacterSession(CharacterSession *session);
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
#endif // CHARACTERS_H