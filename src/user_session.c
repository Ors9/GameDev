#include "characters.h"



struct UserSession
{
    Player *player;
    char name[32];
    char uid[32];
    bool isConnected;
};

