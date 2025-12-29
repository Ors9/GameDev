#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <libpq-fe.h>
#include <stdlib.h>
#include <stdbool.h>


PGconn * ConnectToDatabase();
bool CheckIfUserExists(const char *name , PGconn * dataBase);
bool AddUserToDatabase(const char *name, const char *pass, PGconn *dataBase);
bool ConnectToGame(const char *name, const char *pass, PGconn *dataBase);

#endif