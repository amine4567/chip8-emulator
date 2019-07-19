#include <iostream>

#include "Chip8.h"

using namespace std;

Chip8 myChip8;

int main()
{
    // // Set up render system and register input callbacks
    // myChip8.setupGraphics();
    // myChip8.setupInput();

    // // Initialize the Chip8 system and load the game into the memory
    // myChip8.initialize();
    // myChip8.loadGame("pong");

    // // Emulation loop
    // for (;;)
    // {
    //     // Emulate one cycle
    //     myChip8.emulateCycle();

    //     // If the draw flag is set, update the screen
    //     if (myChip8.drawFlag)
    //         drawGraphics();

    //     // Store key press state (Press and Release)
    //     myChip8.setKeys();
    // }

    cout << "Hello world! Yes I did it !" << endl;

    return 0;
}