#include <game_types.h>
#include <character_session.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct CharacterSession
{
    int cid;
    char cname[32];
    CharacterClass class_type;
    int level;
    int xp;
};

CharacterSession *InitCharacterSession(int cid,char *cname , CharacterClass class_type, int level, int xp)
{
    CharacterSession *session = malloc(sizeof(CharacterSession));
    if (session == NULL)
    {
        printf("Failed to allocate memory for CharacterSession.\n");
        exit(1);
    }
    session->cid = cid;
    session->class_type = class_type;
    session->level = level;
    session->xp = xp;
    strncpy(session->cname, cname, sizeof(session->cname) - 1);
    session->cname[sizeof(session->cname) - 1] = '\0';
    return session;
}

CharacterClass GetCharacterClass(CharacterSession *session)
{
    return session->class_type;
}

void UpdateCharacterSession(CharacterSession *session, char *cname, int cid, CharacterClass class_type, int level, int xp)
{
    session->cid = cid;
    session->class_type = class_type;
    session->level = level;
    session->xp = xp;
    strncpy(session->cname, cname, sizeof(session->cname) - 1);
    session->cname[sizeof(session->cname) - 1] = '\0';
}

CharacterSession *CreateEmptyCharacterList(int size)
{

    CharacterSession *chars = malloc(sizeof(CharacterSession) * size);
    if (chars == NULL)
    {
        perror("Failed to allocate CharacterSession list");
        return NULL;
    }

    for (int i = 0; i < size; i++)
    {
        chars[i].cid = -1;
        chars[i].class_type = -1;
        chars[i].level = 0;
        chars[i].xp = 0;
        chars[i].cname[0] = '\0';
    }
    return chars;
}

CharacterSession* GetCharacterFromList(CharacterSession* list, int index) {
    return &list[index]; 
}

void UnloadCharacterSession(CharacterSession *session)
{
    if (session != NULL)
    {
        free(session);
        session = NULL;
        printf("CharacterSession memory cleared successfully.\n");
    }
}

char *GetCharacterName(CharacterSession *session)
{
    return session->cname;
}

int GetCharacterLevel(CharacterSession *session)
{
    return session->level;
}
