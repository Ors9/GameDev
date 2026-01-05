#include "characters.h"
#include <user_session.h>
#include "stdlib.h"
#include "stdio.h"
#include <string.h>
#include <character_session.h>

struct UserSession
{
    char name[32];
    int uid;
    bool isConnected;
    CharacterSession *character;
};

void UpdateUserSession(UserSession *session, const char *name, int userId, bool isConnected)

{

    strncpy(session->name, name, sizeof(session->name) - 1);
    session->name[sizeof(session->name) - 1] = '\0';

    session->uid = userId;
    session->isConnected = isConnected;
}

UserSession *InitUserSession(const char *name, int uid, bool isConnected, CharacterSession *character)
{
    UserSession *session = malloc(sizeof(UserSession));
    if (session == NULL)
    {
        printf("Failed to allocate memory for UserSession.\n");
        exit(1);
    }

    session->name[0] = '\0';
    session->uid = -1;
    session->isConnected = false;

    if (name != NULL)
    {
        strncpy(session->name, name, sizeof(session->name) - 1);
        session->name[sizeof(session->name) - 1] = '\0';
    }

    session->uid = uid;
    session->isConnected = isConnected;

    session->character = character;

    return session;
}

CharacterSession * GetCharacterSession(UserSession *session){
    return session->character;
}




void UnloadUserSession(UserSession *session)
{

    if (session->character != NULL)
    {
        UnloadCharacterSession(session->character);
        session->character = NULL;
    }

    if (session != NULL)
    {
        free(session);
        session = NULL;
        printf("UserSession memory cleared successfully.\n");
    }
}

int GetUserIdFromSession(UserSession *session)
{
    return session->uid;
}
