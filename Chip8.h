#include <SDL2/SDL.h>

const int MEM_SIZE = 4096;
const int REGISTERS_SIZE = 16;
const int SCREEN_W = 64;
const int SCREEN_H = 32;
const int STACK_SIZE = 16;
const int KEYPAD_INPUTS = 16;

class Chip8
{

public:
    void initialize();
    void emulateCycle();
    void setupGraphics(int);
    void clearGraphics();
    void setupInput();
    void loadGame(std::string);

private:
    unsigned short opcode; // current opcode

    unsigned char memory[MEM_SIZE]; // RAM

    unsigned char V[REGISTERS_SIZE]; // registers

    unsigned short I;  // index register
    unsigned short pc; // program counter

    unsigned char gfx[SCREEN_W * SCREEN_H]; // graphics: boolean pixels (monochromatic)

    unsigned char delay_timer;
    unsigned char sound_timer;

    unsigned short stack[STACK_SIZE];
    unsigned short sp; // stack pointer

    unsigned char key[KEYPAD_INPUTS]; // keypad

    SDL_Renderer *renderer;
};