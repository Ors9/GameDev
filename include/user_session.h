#ifndef USER_SESSION_H
#define USER_SESSION_H

#include <stdbool.h>

typedef struct UserSession UserSession;
typedef struct CharacterSession CharacterSession;


UserSession *InitUserSession(const char *name, int uid, bool isConnected);

void SetUserCharacterList(UserSession *session, CharacterSession *list, int count);


void UnloadUserSession(UserSession *session);
void UpdateUserSession(UserSession *session, const char *name, int userId, bool isConnected);
int GetUserIdFromSession(UserSession *session);
CharacterSession* GetSessionCharacterList(UserSession *session);
int GetSessionCharacterCount(UserSession *session);

void PrintUserSessionDebug(UserSession *session);


CharacterSession *GetCharacterSession(UserSession *session);

#endif // USER_SESSION_H