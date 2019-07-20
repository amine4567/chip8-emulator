#include <iostream>
#include <chrono>
#include <thread>
#include <map>

#include <SDL2/SDL.h>

#include "Chip8.cpp"

using namespace std;

Chip8 myChip8;

int main()
{
    const Uint8 *keyboard_state;

    // Set up render system and register input callbacks
    myChip8.setupGraphics(50);
    myChip8.setupInput();

    // Initialize the Chip8 system and load the game into the memory
    myChip8.initialize();

    myChip8.loadGame("TETRIS");

    int continuer = 1;
    SDL_Event event;
    while (continuer)
    {
        myChip8.emulateCycle();

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                continuer = 0;
                break;
            }
        }

        keyboard_state = SDL_GetKeyboardState(NULL);
        for (std::map<Uint8, unsigned char>::iterator it = myChip8.keypadMap.begin(); it != myChip8.keypadMap.end(); it++)
        {
            if (keyboard_state[it->first])
            {
                cout << it->first << " is pressed" << endl;
                myChip8.key[it->second] = 1;
            }
            else
            {
                myChip8.key[it->second] = 0;
            }
        }

        // If the draw flag is set, update the screen
        if (myChip8.drawFlag == 1)
        {
            myChip8.drawGraphics();
        }

        // Sleep to slow down emulation speed
        std::this_thread::sleep_for(std::chrono::microseconds(2000));
    }

    myChip8.clearGraphics();

    return 0;
}