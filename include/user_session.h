#ifndef USER_SESSION_H
#define USER_SESSION_H

typedef struct UserSession UserSession;
typedef struct CharacterSession CharacterSession;



UserSession *InitUserSession(const char *name, int uid, bool isConnected , CharacterSession *character);
void UnloadUserSession(UserSession *session);
void UpdateUserSession(UserSession *session , const char *name, int userId, bool isConnected);
int GetUserIdFromSession(UserSession *session);
CharacterSession * GetCharacterSession(UserSession *session);


#endif // USER_SESSION_H