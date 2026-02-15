#include <iostream> 
#include <cstdlib>  
#include "raylib.h"
#include "game_loop.h"
#include "sodium.h"

int main()
{

    if (sodium_init() == -1)
    {

        std::cerr << "Fatal Error: libsodium initialization failed!" << std::endl;
        return 1; // בתוך main, עדיף להחזיר 1 מאשר לקרוא ל-exit
    }

    StartGame();

    return 0;
}