#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>


typedef enum {
    AUTH_SUCCESS,               // הכל עבר בהצלחה
    NAME_EMPTY,                 // שם משתמש ריק (NULL)
    NAME_TOO_SHORT,             // פחות מ-4 תווים
    NAME_TOO_LONG,              // יותר מ-10 תווים
    NAME_INVALID_COMPLEXITY,    // חסרה אות או ספרה בשם
    PASS_EMPTY,                 // סיסמה ריקה
    PASS_TOO_SHORT,             // פחות מ-4 תווים
    PASS_TOO_LONG,              // יותר מ-10 תווים
    PASS_MISMATCH,              // הסיסמאות לא זהות (strcmp)
    PASS_INVALID_COMPLEXITY,    // חסרה אות או ספרה בסיסמה
    USER_ALREADY_EXISTS,        // שגיאה מה-DB (שם תפוס)
    DATABASE_ERROR,             // שגיאת תקשורת עם PostgreSQL
    AUTH_UNDEFINED_ERROR        // שגיאה כללית לא צפויה
} AUTH_STATUS;

AUTH_STATUS HandleRegisterRules(char *name, char *pass, char *passCheck);

bool HashedPassword(const char *password, char hashedPassword []);


#endif