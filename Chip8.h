#include <map>

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
    void setupAudio();
    void clearGraphics();
    void setupInput();
    void loadGame(std::string);
    void draw_scaled_pixel(int, int);
    void draw_sprite(unsigned char *, unsigned char, unsigned char, unsigned char);
    void refreshRenderer();
    void drawGraphics();
    void setKeys();

    bool drawFlag;
    std::map<Uint8, unsigned char> keypadMap;

    unsigned short pc; // program counter

    unsigned short opcode; // current opcode

    unsigned char memory[MEM_SIZE]; // RAM

    unsigned char V[REGISTERS_SIZE]; // registers
    unsigned short I;                // index register

    unsigned char delay_timer;
    unsigned char sound_timer;

    unsigned short stack[STACK_SIZE];
    unsigned short sp; // stack pointer

    unsigned char key[KEYPAD_INPUTS]; // keypad

    unsigned char gfx[SCREEN_W * SCREEN_H]; // graphics: boolean pixels (monochromatic)
    SDL_Renderer *renderer;
    int screen_ratio;

    Uint32 wav_length;
    Uint8 *wav_buffer;
    SDL_AudioSpec wav_spec;
};