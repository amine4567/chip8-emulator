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
    myChip8.loadGame("test_opcode.ch8");

    // For testing only. To be removed later
    myChip8.draw_scaled_pixel(50, 20);
    myChip8.draw_scaled_pixel(30, 25);
    myChip8.draw_scaled_pixel(40, 6);

    myChip8.refreshRenderer();

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
        case SDL_KEYDOWN:
            int pressed_key = event.key.keysym.sym;
            if (myChip8.keypadMap.find(pressed_key) == myChip8.keypadMap.end())
            {
                cout << "Unknown key pressed" << endl;
            }
            else
            {
                cout << std::hex << "0x0" << +myChip8.keypadMap[pressed_key] << " key pressed down" << endl;
            }
        }
    }

    // Emulation loop
    while (0)
    {
        // Emulate one cycle
        myChip8.emulateCycle();

        // // If the draw flag is set, update the screen
        // if (myChip8.drawFlag)
        //     myChip8.drawGraphics();

        // // Store key press state (Press and Release)
        // myChip8.setKeys();
    }

    myChip8.clearGraphics();

    return 0;
}