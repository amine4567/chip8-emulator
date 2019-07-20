#include <map>
#include <SDL2/SDL.h>

#include "Chip8.h"

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

    drawFlag = 0;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // black background
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // sprites will be drawn in white

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
        memory[i] = 0x00;
    }

    // Load fontset
    for (int j = 0; j < 80; j++)
        memory[j] = chip8_fontset[j];

    // Reset timers
    delay_timer = 0;
    sound_timer = 0;
}

void Chip8::setupGraphics(int scrn_ratio = 10)
{
    screen_ratio = scrn_ratio;
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Chip8 emulator",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_W * screen_ratio,
                                          SCREEN_H * screen_ratio,
                                          SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void Chip8::refreshRenderer()
{
    SDL_RenderPresent(renderer);
}

void Chip8::draw_scaled_pixel(int x, int y)
{
    for (int xi = x * screen_ratio; xi <= x * screen_ratio + screen_ratio - 1; xi++)
        for (int yi = y * screen_ratio; yi <= y * screen_ratio + screen_ratio - 1; yi++)
        {
            SDL_RenderDrawPoint(renderer, xi, yi);
        }
}

void Chip8::draw_sprite(unsigned char *sprite, unsigned char x0, unsigned char y0, unsigned char n)
{
    unsigned char current_x = x0, current_y = y0;

    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            bool bit = sprite[i] & (1 << j);
            if (bit == 1)
            {
                draw_scaled_pixel(current_x, current_y);
            }
            current_x++;
        }
        current_y++;
    }
}

void Chip8::clearGraphics()
{
    SDL_Quit();
}

void Chip8::emulateCycle()
{
    // Fetch opcode
    opcode = memory[pc] << 8 | memory[pc + 1];
    printf("Fetched opcode: 0x%X\n", opcode);

    // Decode opcode
    switch (opcode & 0xF000)
    {
    case 0x0000:
        switch (opcode & 0x0FFF)
        {
        case 0x00E0: //00E0 - CLS. Clear the display.
            for (int i = 0; i <= SCREEN_W * SCREEN_H - 1; i++)
                gfx[i] = 0;
            drawFlag = 1;
            pc += 2;
            break;
        case 0x00EE: // 00EE - RET. Return from a subroutine.
            sp--;
            pc = stack[sp];
            pc += 2;
            break;
        default:
            pc += 2;
        }
        break;
    case 0x1000: //1nnn - JP addr. Jump to location nnn.
        pc = opcode & 0x0FFF;
        break;
    case 0x2000: //2nnn - CALL addr. Call subroutine at nnn.
        stack[sp] = pc;
        sp++;
        pc = opcode & 0x0FFF;
        break;
    case 0x3000: //3xkk - SE Vx, byte. Skip next instruction if Vx = kk.
        if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
        {
            pc += 2;
        }
        pc += 2;
        break;
    case 0x4000: //4xkk - SNE Vx, byte. Skip next instruction if Vx != kk.
        if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
        {
            pc += 2;
        }
        pc += 2;
        break;
    case 0x5000: //5xy0 - SE Vx, Vy. Skip next instruction if Vx = Vy.
        if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
        {
            pc += 2;
        }
        pc += 2;
        break;
    case 0x6000: //6xkk - LD Vx, byte. Set Vx = kk.
        V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
        pc += 2;
        break;
    case 0x7000: //7xkk - ADD Vx, byte. Set Vx = Vx + kk.
        V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
        pc += 2;
        break;
    case 0x8000:
        switch (opcode & 0x000F)
        {
        case 0x0000: //8xy0 - LD Vx, Vy. Set Vx = Vy.
            V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
            pc += 2;
            break;
        case 0x0001: //8xy1 - OR Vx, Vy. Set Vx = Vx OR Vy.
            V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
            pc += 2;
            break;
        case 0x0002: //8xy2 - AND Vx, Vy. Set Vx = Vx AND Vy.
            V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
            pc += 2;
            break;
        case 0x0003: //8xy3 - XOR Vx, Vy. Set Vx = Vx XOR Vy.
            V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
            pc += 2;
            break;
        case 0x0004:
            //8xy4 - ADD Vx, Vy. Set Vx = Vx + Vy, set VF = carry.
            {
                unsigned short sum = V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4];
                (sum > 255) ? V[0xF] = 1 : V[0xF] = 0;
                V[(opcode & 0x0F00) >> 8] = sum & 0x00FF;
                pc += 2;
            }
            break;
        case 0x0005: //8xy5 - SUB Vx, Vy. Set Vx = Vx - Vy, set VF = NOT borrow.
            (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]) ? V[0xF] = 1 : V[0xF] = 0;
            V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
            pc += 2;
            break;
        case 0x0006: //8xy6 - SHR Vx {, Vy}. Set Vx = Vx SHR 1.
            (V[(opcode & 0x0F00) >> 8] & 0x01 == 0x01) ? V[0xF] = 1 : V[0xF] = 0;
            V[(opcode & 0x0F00) >> 8] /= 2;
            pc += 2;
            break;
        case 0x0007: //8xy7 - SUBN Vx, Vy. Set Vx = Vy - Vx, set VF = NOT borrow.
            (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8]) ? V[0xF] = 1 : V[0xF] = 0;
            V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
            pc += 2;
            break;
        case 0x000E: //SHL Vx {, Vy}. Set Vx = Vx SHL 1.
            ((V[(opcode & 0x0F00) >> 8] & 0x80) == 0x80) ? V[0xF] = 1 : V[0xF] = 0;
            V[(opcode & 0x0F00) >> 8] *= 2;
            pc += 2;
            break;
        }
        break;
    case 0x9000: //9xy0 - SNE Vx, Vy. Skip next instruction if Vx != Vy.
        if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
        {
            pc += 2;
        }
        pc += 2;
        break;
    case 0xA000: //Annn - LD I, addr. Set I = nnn.
        I = opcode & 0x0FFF;
        pc += 2;
        break;
    case 0xB000: //Bnnn - JP V0, addr. Jump to location nnn + V0.
        pc = (opcode & 0x0FFF) + V[0x0];
        break;
    case 0xC000:
        //Cxkk - RND Vx, byte. Set Vx = random byte AND kk.
        {
            unsigned char rnd = (unsigned char)rand();
            V[(opcode & 0x0F00) >> 8] = rnd & (opcode & 0x00FF);
        }
        pc += 2;
        break;
    case 0xD000: //Dxyn - DRW Vx, Vy, nibble.
        //Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
        {
            unsigned char n = opcode & 0x000F;

            unsigned char x = V[(opcode & 0x0F00) >> 8];
            unsigned char y = V[(opcode & 0x00F0) >> 4];

            V[0xF] = 0;
            for (int j = 0; j <= n - 1; j++)
            {
                unsigned int gfx_i = (y + j) * SCREEN_W + x;

                for (int k = 0; k < 8; k++)
                {
                    bool bit = memory[I + j] & (0x80 >> k);
                    unsigned char gfx_before = gfx[gfx_i];
                    gfx[gfx_i] = gfx[gfx_i] ^ bit;

                    if (gfx_before == 1 && gfx[gfx_i] == 0)
                    {
                        V[0xF] = 1;
                    }

                    gfx_i++;
                }
            }
            // TODO: If the sprite is positioned so part of it is outside the coordinates of the display,
            // it wraps around to the opposite side of the screen.
            drawFlag = 1;
            pc += 2;
        }
        break;
    case 0xE000:
        switch (opcode & 0x00FF)
        {
        case 0x009E: //Ex9E - SKP Vx.
            //Skip next instruction if key with the value of Vx is pressed.
            if (key[V[(opcode & 0x0F00) >> 8]] != 0)
                pc += 2;
            pc += 2;
            break;
        case 0x00A1: //ExA1 - SKNP Vx.
            //Skip next instruction if key with the value of Vx is not pressed.
            if (key[V[(opcode & 0x0F00) >> 8]] == 0)
                pc += 2;
            pc += 2;
            break;
        }
        break;
    case 0xF000:
        switch (opcode & 0x00FF)
        {
        case 0x0007: //Fx07 - LD Vx, DT. Set Vx = delay timer value.
            V[(opcode & 0x0F00) >> 8] = delay_timer;
            pc += 2;
            break;
        case 0x000A: //Fx0A - LD Vx, K.
            //Wait for a key press, store the value of the key in Vx.
            {
                bool key_pressed = false;

                for (int i = 0; i < 16; ++i)
                {
                    if (key[i] != 0)
                    {
                        V[(opcode & 0x0F00) >> 8] = i;
                        key_pressed = true;
                    }
                }

                // If no key is pressed, return and try again.
                if (!key_pressed)
                    return;

                pc += 2;
            }
            break;
        case 0x0015: //Fx15 - LD DT, Vx. Set delay timer = Vx.
            delay_timer = V[(opcode & 0x0F00) >> 8];
            pc += 2;
            break;
        case 0x0018: //Fx18 - LD ST, Vx. Set sound timer = Vx.
            sound_timer = V[(opcode & 0x0F00) >> 8];
            pc += 2;
            break;
        case 0x001E: //Fx1E - ADD I, Vx. Set I = I + Vx.
            I += V[(opcode & 0x0F00) >> 8];
            pc += 2;
            break;
        case 0x0029: // Fx29 - LD F, Vx. Set I = location of sprite for digit Vx.
            I = V[(opcode & 0x0F00) >> 8] * 0x5;
            pc += 2;
            break;
        case 0x0033: //Fx33 - LD B, Vx.
            //Store BCD representation of Vx in memory locations I, I + 1, and I + 2.
            {
                unsigned char vx = V[(opcode & 0x0F00) >> 8];
                int hundreds_digit = int(vx / 100);
                int tens_digit = int((vx - hundreds_digit * 100) / 10);
                int ones_digit = vx - hundreds_digit * 100 - tens_digit * 10;

                memory[I] = hundreds_digit;
                memory[I + 1] = tens_digit;
                memory[I + 2] = ones_digit;

                pc += 2;
            }
            break;
        case 0x0055: //Fx55 - LD [I], Vx.
            //Store registers V0 through Vx in memory starting at location I.
            {
                unsigned char x = (opcode & 0x0F00) >> 8;

                for (unsigned char i = 0; i <= x; i++)
                {
                    memory[I + i] = V[i];
                }
                pc += 2;
            }
            break;
        case 0x0065: //Fx65 - LD Vx, [I].
            //read registers V0 through Vx from memory starting at location I.
            {
                unsigned char x = (opcode & 0x0F00) >> 8;

                for (unsigned char i = 0; i <= x; i++)
                {
                    V[i] = memory[I + i];
                }
                pc += 2;
            }
            break;
        }
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
    int c;
    int bufferSize = 0;

    int i = -1;
    while ((c = fgetc(pFile)) != EOF)
    {
        bufferSize++;
        i++;
        memory[i + 512] = c;
    }

    cout << "bufferSize: " << bufferSize << endl;
}

void Chip8::setupInput()
{
    keypadMap.insert(pair<int, unsigned char>(SDLK_1, 0x01));
    keypadMap.insert(pair<int, unsigned char>(SDLK_2, 0x02));
    keypadMap.insert(pair<int, unsigned char>(SDLK_3, 0x03));
    keypadMap.insert(pair<int, unsigned char>(SDLK_4, 0x0C));
    keypadMap.insert(pair<int, unsigned char>(SDLK_a, 0x04));
    keypadMap.insert(pair<int, unsigned char>(SDLK_z, 0x05));
    keypadMap.insert(pair<int, unsigned char>(SDLK_e, 0x06));
    keypadMap.insert(pair<int, unsigned char>(SDLK_r, 0x0D));
    keypadMap.insert(pair<int, unsigned char>(SDLK_q, 0x07));
    keypadMap.insert(pair<int, unsigned char>(SDLK_s, 0x08));
    keypadMap.insert(pair<int, unsigned char>(SDLK_d, 0x09));
    keypadMap.insert(pair<int, unsigned char>(SDLK_f, 0x0E));
    keypadMap.insert(pair<int, unsigned char>(SDLK_w, 0x0A));
    keypadMap.insert(pair<int, unsigned char>(SDLK_x, 0x00));
    keypadMap.insert(pair<int, unsigned char>(SDLK_c, 0x0B));
    keypadMap.insert(pair<int, unsigned char>(SDLK_v, 0x0F));

    for (int i = 0; i <= KEYPAD_INPUTS - 1; i++)
    {
        key[i] = 0;
    }
}

void Chip8::drawGraphics()
{
    for (int x = 0; x < SCREEN_W; x++)
        for (int y = 0; y < SCREEN_H; y++)
        {
            char px_val = gfx[y * SCREEN_W + x];
            if (px_val == 1)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            }
            draw_scaled_pixel(x, y);
        }

    SDL_RenderPresent(renderer);
    drawFlag = 0;
}

void setKeys()
{
}