#include <SDL2/SDL.h>

#include "tools.h"

void sdl_loop()
{
    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            continuer = 0;
        }
    }
}

void draw_scaled_pixel(SDL_Renderer *renderer, int x, int y, int screen_ratio)
{
    for (int xi = x * screen_ratio; xi <= x * screen_ratio + screen_ratio - 1; xi++)
        for (int yi = y * screen_ratio; yi <= y * screen_ratio + screen_ratio - 1; yi++)
        {
            SDL_RenderDrawPoint(renderer, xi, yi);
        }
}

void draw_sprite(unsigned char *sprite, unsigned char x, unsigned char y, unsigned char n)
{
}