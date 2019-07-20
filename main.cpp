#include <iostream>

#include <SDL2/SDL.h>

#include "Chip8.cpp"

using namespace std;

Chip8 myChip8;

int main()
{
    // // Set up render system and register input callbacks
    myChip8.setupGraphics(50);
    myChip8.setupInput();

    // Initialize the Chip8 system and load the game into the memory
    myChip8.initialize();
    // cout << "program counter: " << myChip8.pc << endl;

    myChip8.loadGame("TETRIS");

    int continuer = 1;
    SDL_Event event;
    while (continuer)
    {
        SDL_PollEvent(&event); /* Récupération de l'événement dans event */
        switch (event.type)    /* Test du type d'événement */
        {
        case SDL_QUIT: /* Si c'est un événement de type "Quitter" */
            continuer = 0;
            break;
        case SDL_KEYUP:
            int pressed_key = event.key.keysym.sym;
            if (myChip8.keypadMap.find(pressed_key) == myChip8.keypadMap.end())
            {
                cout << "Unknown key pressed and released" << endl;
            }
            else
            {
                unsigned char chip8_key = myChip8.keypadMap.find(pressed_key)->second;
                cout << std::hex << "0x0" << +chip8_key << " key pressed down" << endl;
                myChip8.key[chip8_key] = 1;
            }
        }

        myChip8.emulateCycle();

        // If the draw flag is set, update the screen
        if (myChip8.drawFlag == 1)
        {
            myChip8.drawGraphics();
        }
    }

    myChip8.clearGraphics();

    return 0;
}