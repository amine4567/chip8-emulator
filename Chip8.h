class Chip8
{

public:
    void initialize();
    void emulateCycle();
    void setupGraphics();
    void setupInput();

private:
    unsigned short opcode; // current opcode

    unsigned char memory[4096]; // RAM

    unsigned char V[16]; // registers

    unsigned short I;  // index register
    unsigned short pc; // program counter

    unsigned char gfx[64 * 32]; // graphics: boolean pixels (monochromatic)

    unsigned char delay_timer;
    unsigned char sound_timer;

    unsigned short stack[16];
    unsigned short sp; // stack pointer

    unsigned char key[16]; // keypad
};