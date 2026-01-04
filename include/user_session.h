#ifndef USER_SESSION_H
#define USER_SESSION_H

typedef struct UserSession UserSession;


UserSession * InitUserSession(const char *name, int uid, bool isConnected);
void UnloadUserSession(UserSession *session);
void UpdateUserSession(UserSession *session , const char *name, int userId, bool isConnected);
int GetUserIdFromSession(UserSession *session);

#endif // USER_SESSION_H