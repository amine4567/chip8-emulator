#include <SDL2/SDL.h>

#include "Chip8.h"
#include "tools.h"

using namespace std;

unsigned char chip8_fontset[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void Chip8::initialize()
{
    pc = 0x200; // Program counter starts at 0x200
    opcode = 0; // Reset current opcode
    I = 0;      // Reset index register
    sp = 0;     // Reset stack pointer

    // Clear display
    for (int i = 0; i <= SCREEN_W * SCREEN_H - 1; i++)
    {
        gfx[i] = 0;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // black background
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // content will be drawn in white

    // Clear stack
    for (int i = 0; i <= STACK_SIZE - 1; i++)
    {
        stack[i] = 0;
    }

    // Clear registers V0-VF
    for (int i = 0; i <= REGISTERS_SIZE - 1; i++)
    {
        V[i] = 0;
    }

    // Clear memory
    for (int i = 0; i <= MEM_SIZE - 1; i++)
    {
        memory[i] = 0;
    }

    // Load fontset
    for (int i = 0; i < 80; ++i)
        memory[i] = chip8_fontset[i];

    // Reset timers
    delay_timer = 0;
    sound_timer = 0;
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
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // SDL_RenderClear(renderer);
    // SDL_SetRenderDrawColor(renderer, 255, 0, 100, 255);

    // draw_scaled_pixel(renderer, 50, 20, screen_ratio);
    // draw_scaled_pixel(renderer, 30, 25, screen_ratio);
    // draw_scaled_pixel(renderer, 40, 6, screen_ratio);

    // SDL_RenderPresent(renderer);
}

void Chip8::clearGraphics()
{
    SDL_Quit();
}

void Chip8::emulateCycle()
{
    // Fetch opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    // Decode opcode
    switch (opcode & 0xF000)
    {
        // Some opcodes //
    case 0x1000: //1NNN - JP addr. Jump to location NNN.
        pc = opcode & 0x0FFF;
        break;
    case 0x2000: //2NNN - CALL addr. Call subroutine at NNN.
        stack[sp] = pc;
        sp++;
        pc = opcode & 0x0FFF;
        break;
    case 0x3000: //3XKK - SE VX, byte. Skip next instruction if VX = KK.
        if (V[opcode & 0x0F00] == opcode & 0x00FF)
        {
            pc += 2;
        }
        break;
    case 0xA000: // ANNN: Sets I to the address NNN
        I = opcode & 0x0FFF;
        pc += 2;
        break;

    default:
        printf("Unknown opcode: 0x%X\n", opcode);
    }

    // Update timers
    if (delay_timer > 0)
        --delay_timer;

    if (sound_timer > 0)
    {
        if (sound_timer == 1)
            printf("BEEP!\n");
        --sound_timer;
    }
}

void Chip8::loadGame(std::string game_name)
{
    FILE *pFile;

    std::string filename = "games/" + game_name;
    char cstr_filename[filename.size() + 1];
    strcpy(cstr_filename, filename.c_str());

    pFile = fopen(cstr_filename, "rb");
    unsigned char c;
    int bufferSize = 0;

    int i = -1;
    while (c = fgetc(pFile) != EOF)
    {
        bufferSize++;
        i++;
        memory[i + 512] = c;
    }

    cout << "bufferSize: " << bufferSize << endl;
}