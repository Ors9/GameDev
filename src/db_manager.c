#include "db_manager.h"
#include "stdbool.h"
#include "auth.h"
#include "sodium.h"
#include <string.h>

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

bool ConnectToGame(const char *name, const char *pass, PGconn *dataBase)
{
    // 1. שאילתה ספציפית לעמודה של ה-Hash
    const char *sql = "SELECT password_hash FROM users WHERE username = $1";

    // 2. הגדרה תקינה של מערך הפרמטרים ב-C
    const char *params[1];
    params[0] = name;

    // 3. ביצוע השאילתה
    PGresult *res = PQexecParams(dataBase, sql, 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Query failed: %s\n", PQerrorMessage(dataBase));
        PQclear(res);
        return false;
    }

    // 4. בדיקה אם המשתמש קיים
    if (PQntuples(res) == 0)
    {
        // כאן תוכל לעדכן את ui.errorMsg במסך ה-Login
        printf("User Name not found\n");
        PQclear(res);
        return false;
    }

    // 5. שליפת ה-Hash (עמודה 0 כי ביקשנו רק אותה ב-SELECT)
    const char *dbHash = PQgetvalue(res, 0, 0);

    // 6. אימות הסיסמה עם libsodium
    if (crypto_pwhash_str_verify(dbHash, pass, strlen(pass)) != 0)
    {
        PQclear(res);
        printf("Password does not match\n");
        return false;
    }

    // 7. הצלחה!
    PQclear(res);
    printf("Success login!\n");
    return true;
}

bool AddUserToDatabase(const char *name, const char *pass, PGconn *dataBase)
{
    const char *sql = "Insert into users (username,password_hash) values ($1,$2)";
    char hashedPass[crypto_pwhash_STRBYTES];
    if (HashedPassword(pass, hashedPass) == false)
    {
        printf("To do update UI message error");
        fprintf(stderr, "Failed to hash password\n");
        return false;
    }
    const char *paramValues[2];
    paramValues[0] = name;
    paramValues[1] = hashedPass;

    PGresult *res = PQexecParams(dataBase, sql, 2, NULL, paramValues, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "DB Error: %s\n", PQerrorMessage(dataBase));
        PQclear(res);
        return false;
    }

    PQclear(res);
    printf("Success register! TO DO UI message");
    return true;
}

bool CheckIfUserExists(const char *name, PGconn *dataBase)
{
    const char *sql = "select * from users where username = $1";

    const char *paramValues[1];
    paramValues[0] = name;

    PGresult *res = PQexecParams(dataBase, sql, 1, NULL, paramValues, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Query failed: %s\n", PQerrorMessage(dataBase));
        PQclear(res);
        return false;
    }

    int rows = PQntuples(res);

    PQclear(res);

    return rows > 0;
}