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
    for (int i = 0; i < 80; ++i)
        memory[i] = chip8_fontset[i];

    // Reset timers
}

void Chip8::setupGraphics()
{
    // Fetch Opcode
    // Decode Opcode
    // Execute Opcode

    // Update timers
}

void Chip8::emulateCycle()
{
    // Fetch Opcode
    // Decode Opcode
    // Execute Opcode

    // Update timers
}