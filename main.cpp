#include <iostream>

#include "Chip8.cpp"
#include "tools.cpp"

using namespace std;

Chip8 myChip8;

int main()
{
    // // Set up render system and register input callbacks
    myChip8.setupGraphics(20);
    sdl_loop();
    
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

    cout << "Hello world! Yes I did it !" << endl << "lalamdfkds" << endl;

    myChip8.clearGraphics();
    
    return 0;
}