#include <SDL2/SDL.h>

#include "Chip8.h"
#include "tools.h"

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
                                          SCREEN_W * screen_ratio,
                                          SCREEN_H * screen_ratio,
                                          SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 100, 255);

    draw_scaled_pixel(renderer, 50, 20, screen_ratio);
    draw_scaled_pixel(renderer, 30, 25, screen_ratio);
    draw_scaled_pixel(renderer, 40, 6, screen_ratio);

    SDL_RenderPresent(renderer);
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