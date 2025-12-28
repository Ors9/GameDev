#include "raylib.h"
#include "game_loop.h"
#include <stdlib.h>
#include <stdio.h>
#include "sodium.h"


int main()
{
    if(sodium_init() == -1){
        printf("Sodium init fail at main");
        exit(1);
    }
    StartGame();
    return 0;
}