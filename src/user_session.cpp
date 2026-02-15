#include "characters.h"
#include <user_session.h>
#include "stdlib.h"
#include "stdio.h"
#include <string.h>
#include <character_session.h>





// המבנה המעודכן כפי שהחלטנו
struct UserSession
{
    char name[32];
    int uid;
    bool isConnected;
    
    CharacterSession *characterList; // רשימת האפשרויות
    int characterCount;
    CharacterSession *activeCharacter; // הדמות שנבחרה למשחק
};



void PrintUserSessionDebug(UserSession *session) {
    if (session == nullptr) {
        printf("DEBUG: UserSession is nullptr\n");
        return;
    }

    printf("\n=== [USER SESSION DEBUG] ===\n");
    printf("User Name:      %s\n", session->name);
    printf("User ID:        %d\n", session->uid);
    printf("Connected:      %s\n", session->isConnected ? "YES" : "NO");
    
    // בדיקת רשימת הדמויות
    if (session->characterList == nullptr) {
        printf("Char List:      EMPTY (nullptr)\n");
    } else {
        printf("Char List:      LOADED (Count: %d, Address: %p)\n", 
               session->characterCount, (void*)session->characterList);
    }

    // בדיקת הדמות הפעילה
    if (session->activeCharacter != nullptr) {
        printf("Active Char:    %s (LVL: %d, CID: %d)\n", 
               GetCharacterName(session->activeCharacter),
               GetCharacterLevel(session->activeCharacter),
               GetCharacterId(session->activeCharacter));
    } else {
        printf("Active Char:    NONE (nullptr)\n");
    }
    printf("============================\n\n");
}

// אתחול הסשן - עכשיו מאתחלים גם את הרשימה כריקה
UserSession *InitUserSession(const char *name, int uid, bool isConnected)
{
    UserSession *session = new UserSession;
    if (session == nullptr)
    {
        printf("Failed to allocate memory for UserSession.\n");
        exit(1);
    }

    // ניקוי זיכרון ראשוני
    memset(session, 0, sizeof(UserSession));

    if (name != nullptr)
    {
        strncpy(session->name, name, sizeof(session->name) - 1);
        session->name[sizeof(session->name) - 1] = '\0';
    }

    session->uid = uid;
    session->isConnected = isConnected;

    // מאתחלים את שדות הדמויות כריקים
    session->characterList = nullptr;
    session->characterCount = 0;
    session->activeCharacter = nullptr;

    return session;
}

int GetUserIdFromSession(UserSession *session)
{
    return session->uid;
}

void UpdateUserSession(UserSession *session, const char *name, int userId, bool isConnected)
{
    if (session == nullptr) return;

    // עדכון שם המשתמש בבטחה
    if (name != nullptr) {
        strncpy(session->name, name, sizeof(session->name) - 1);
        session->name[sizeof(session->name) - 1] = '\0';
    }

    session->uid = userId;
    session->isConnected = isConnected;

}

// מחזיר את הדמות הפעילה כרגע במשחק
CharacterSession *GetCharacterSession(UserSession *session)
{
    return session->activeCharacter;
}

// פונקציה חדשה שתצטרך למסך הבחירה
CharacterSession *GetUserCharacterList(UserSession *session, int *count)
{
    if (count)
        *count = session->characterCount;
    return session->characterList;
}

// 1. הגדרת רשימת הדמויות ב-Session
void SetUserCharacterList(UserSession *session, CharacterSession *list, int count) 
{
    if (session == nullptr) return;

    // אם כבר הייתה רשימה קודמת (למשל נכנסנו ויצאנו מהמסך), ננקה אותה קודם
    if (session->characterList != nullptr) {
        UnloadCharacterSession(session->characterList);
    }

    session->characterList = list;
    session->characterCount = count;
    session->activeCharacter = nullptr; // איפוס הדמות הפעילה עד לבחירה
}

// 2. בחירת דמות וניקוי המיותרים
void FinalizeCharacterSelection(UserSession *session, int selectedIndex , AssetManager *assets) 
{
    if (session == nullptr || session->characterList == nullptr) return;
    if (selectedIndex < 0 || selectedIndex >= session->characterCount) return;

    // שליפת הדמות שנבחרה מתוך הרשימה
    CharacterSession *chosenFromList = GetCharacterFromList(session->characterList, selectedIndex);

    // לוגיקת הניקוי: 
    // אנחנו יוצרים עותק נפרד ויחיד של הדמות כדי שנוכל למחוק את הרשימה כולה
    session->activeCharacter = CloneCharacterSession(chosenFromList);

    //
    InitCharacterPlayer(session->activeCharacter ,assets);

    // עכשיו אפשר למחוק את כל הרשימה הזמנית שנטענה מה-DB
    UnloadCharacterSession(session->characterList);
    
    // איפוס מצביעים כדי שלא ננסה לגשת אליהם שוב
    session->characterList = nullptr;
    session->characterCount = 0;

    printf("Character '%s' selected. Memory for other options cleared.\n", GetCharacterName(session->activeCharacter));
}

CharacterSession* GetSessionCharacterList(UserSession *session) {
    if (session == nullptr) return nullptr;
    return session->characterList;
}

int GetSessionCharacterCount(UserSession *session) {
    if (session == nullptr) return 0;
    return session->characterCount;
}

void UnloadUserSession(UserSession *session)
{
    if (session == nullptr)
        return;

    // שחרור רשימת הדמויות (אם קיימת)
    if (session->characterList != nullptr)
    {
        // כאן תצטרך לולאה שמשחררת כל איבר ברשימה
        // אלא אם כן UnloadCharacterSession מטפלת במערך שלם
        UnloadCharacterSession(session->characterList);
        session->characterList = nullptr;
    }

    // הדמות הפעילה היא בדרך כלל חלק מהרשימה,
    // אז אחרי ששחררנו את הרשימה, פשוט נאפס את המצביע
    session->activeCharacter = nullptr;

    delete session;
    printf("UserSession memory cleared successfully.\n");
}