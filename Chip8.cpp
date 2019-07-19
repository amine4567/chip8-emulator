#include <SDL2/SDL.h>

#include "Chip8.h"

using namespace std;

void Chip8::initialize()
{
    pc = 0x200; // Program counter starts at 0x200
    opcode = 0; // Reset current opcode
    I = 0;      // Reset index register
    sp = 0;     // Reset stack pointer

    // Clear display
    // Clear stack
    // Clear registers V0-VF
    // Clear memory

    // Load fontset
    // for (int i = 0; i < 80; ++i)
    //     memory[i] = chip8_fontset[i];

    // Reset timers
}

void Chip8::setupGraphics(int screen_ratio = 10)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Chip8 emulator", 
                          SDL_WINDOWPOS_UNDEFINED, 
                          SDL_WINDOWPOS_UNDEFINED, 
                          SCREEN_W*screen_ratio, SCREEN_H*screen_ratio, 
                          SDL_WINDOW_OPENGL);

    if (window == NULL) {
        cout << "Could not create windows:" << SDL_GetError() << endl; 
    }
}

void Chip8::clearGraphics()
{
    SDL_Quit();
}

void Chip8::emulateCycle()
{
    // Fetch Opcode
    // Decode Opcode
    // Execute Opcode

    // Update timers
}