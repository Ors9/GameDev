#include "db_manager.h"

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
