#include <game_types.h>
#include <character_session.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <characters.h>

struct CharacterSession
{
    int cid;
    char cname[32];
    Player *player;
    CharacterClass class_type;
    int level;
    int xp;
};

Player* GetPlayer(CharacterSession *cs){
    return cs->player;
}

void PrintCharacterSession(CharacterSession *session)
{
    if (session == NULL)
    {
        printf("[CharacterSession] Error: Attempted to print a NULL session.\n");
        return;
    }

    printf("--- Character Session Info ---\n");
    printf("ID:      %d\n", session->cid);
    printf("Name:    %s\n", session->cname);
    printf("Class:   %d\n", (int)session->class_type); // Cast to int for safety
    printf("Level:   %d\n", session->level);
    printf("XP:      %d\n", session->xp);
    
    if (session->player != NULL) {
        printf("Status:  Active (Player object initialized)\n");
    } else {
        printf("Status:  Inactive (No Player object)\n");
    }
    printf("------------------------------\n");
}

CharacterSession *InitCharacterSession(int cid, char *cname, CharacterClass class_type, int level, int xp)
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
    session->player = NULL;
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

void InitCharacterPlayer(CharacterSession *session, AssetManager *assets)
{
    session->player = InitPlayer(session->class_type, assets);
    PrintCharacterSession(session);
    
}

CharacterSession *GetCharacterFromList(CharacterSession *list, int index)
{
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

// תוסיף את זה ל-src/character_session.c
CharacterSession *CloneCharacterSession(CharacterSession *source)
{
    if (source == NULL)
        return NULL;

    // יוצר רשימה חדשה של איבר אחד
    CharacterSession *newNode = CreateEmptyCharacterList(1);

    // מעתיק את כל הנתונים מהמקור
    UpdateCharacterSession(
        newNode,
        GetCharacterName(source),
        GetCharacterId(source),
        GetCharacterClass(source),
        GetCharacterLevel(source),
        GetCharacterXP(source));

    return newNode;
}

int GetCharacterId(CharacterSession *session)
{
    if (session == NULL)
        return -1;
    return session->cid; // וודא שזה השם של השדה אצלך ב-struct (כנראה cid לפי ה-SQL)
}

int GetCharacterXP(CharacterSession *session)
{
    if (session == NULL)
        return 0;
    return session->xp; // וודא שזה השם של השדה אצלך
}
