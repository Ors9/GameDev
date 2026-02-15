#include "auth/db_auth_manager.h"
#include "stdbool.h"
#include "auth/auth.h"
#include "sodium.h"
#include <string.h>
#include <game_state.h>
#include <user_session.h>

PGconn *ConnectToDatabase()
{
    const char *conninfo = "host=localhost port=5432 dbname=summoner_game user=postgres password=1234 connect_timeout=10";
    PGconn *conn = PQconnectdb(conninfo);

    if (conn == NULL || PQstatus(conn) == CONNECTION_BAD)
    {
        fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }
    printf("Successfully connected to database: %s\n", PQdb(conn));
    return conn;
}

bool ConnectToGame(const char *name, const char *pass, PGconn *dataBase, char *outErrorMsg, GameState *gs)
{
    // 1. השאילתה - רק $1 (עבור ה-Username)
    const char *sql = "SELECT id, password_hash FROM users WHERE username = $1";

    // 2. רק פרמטר אחד נשלח ל-DB
    const char *params[1];
    params[0] = name;

    // 3. ביצוע השאילתה - שים לב לשינוי למספר 1 בפרמטרים
    PGresult *res = PQexecParams(dataBase, sql, 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        strncpy(outErrorMsg, "Database error.", 127);
        PQclear(res);
        return false;
    }

    // 4. בדיקה אם המשתמש קיים
    if (PQntuples(res) == 0)
    {
        strncpy(outErrorMsg, "User name does not exist", 127);
        PQclear(res);
        return false;
    }

    // 5. שליפת הנתונים
    int userId = atoi(PQgetvalue(res, 0, 0));   // עמודה 0: id
    const char *dbHash = PQgetvalue(res, 0, 1); // עמודה 1: password_hash

    // 6. אימות הסיסמה עם libsodium
    if (crypto_pwhash_str_verify(dbHash, pass, strlen(pass)) != 0)
    {
        strncpy(outErrorMsg, "Invalid password!", 127);
        PQclear(res);
        return false;
    }

    UpdateUserSession(GetUserSession(gs), name, userId, true);

    PQclear(res);
    sodium_memzero((void *)pass, strlen(pass));

    printf("Success login! Logged in as: %s (ID: %d)\n", name, userId);
    outErrorMsg[0] = '\0';
    return true;
}

bool AddUserToDatabase(const char *name, const char *pass, PGconn *dataBase, char *outErrorMsg)
{
    const char *sql = "Insert into users (username,password_hash) values ($1,$2)";
    char hashedPass[crypto_pwhash_STRBYTES];
    if (HashedPassword(pass, hashedPass) == false)
    {
        strncpy(outErrorMsg, "Critical: Could not secure password.", 127);
        return false;
    }
    const char *paramValues[2];
    paramValues[0] = name;
    paramValues[1] = hashedPass;

    PGresult *res = PQexecParams(dataBase, sql, 2, NULL, paramValues, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        strncpy(outErrorMsg, "Registration failed: Server error.", 127);
        PQclear(res);
        return false;
    }

    PQclear(res);
    sodium_memzero((void *)pass, strlen(pass));
    outErrorMsg[0] = '\0';
    return true;
}

bool CheckIfUserExists(const char *name, PGconn *dataBase, char *outErrorMsg)
{
    const char *sql = "SELECT 1 FROM users WHERE username = $1";
    const char *paramValues[1] = {name};

    PGresult *res = PQexecParams(dataBase, sql, 1, NULL, paramValues, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        strncpy(outErrorMsg, "Database link failed.", 127);
        PQclear(res);
        return false;
    }

    int rows = PQntuples(res);
    PQclear(res);

    if (rows > 0)
    {
        strncpy(outErrorMsg, "Username is already taken!", 127);
        return true;
    }

    outErrorMsg[0] = '\0';
    return false;
}