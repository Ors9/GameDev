#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <libpq-fe.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct GameState GameState; // game_state.c struct

PGconn * ConnectToDatabase();
bool CheckIfUserExists(const char *name , PGconn * dataBase , char *outErrorMsg);
bool AddUserToDatabase(const char *name, const char *pass, PGconn *dataBase , char *outErrorMsg);
bool ConnectToGame(const char *name, const char *pass, PGconn *dataBase, char *outErrorMsg , GameState * gs);

#endif // DB_MANAGER_H