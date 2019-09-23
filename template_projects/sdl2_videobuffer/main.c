#include <SDL2/SDL.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "shorttypes.h"
#define D_WIDTH  640
#define D_HEIGHT 480

/*
 Simple POSIX timer, for delta time measurement
 NOTE: Don't try yo compile without gnu's version of C.
*/
#include <time.h>
struct timespec timespec_diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}
typedef struct { struct timespec last;} Timer;
double Timer_get_elapsed_sec(Timer *const t)
{
    struct timespec now;
    struct timespec delta_timespec;
    clock_gettime(CLOCK_MONOTONIC, &now);
    delta_timespec = timespec_diff(t->last, now);
    double delta = (double)delta_timespec.tv_nsec /(double) 1000000000.0;
    //return delta_timespec.tv_sec;
    return delta;
}
internal inline void Timer_reset(Timer *const t) { clock_gettime(CLOCK_MONOTONIC, &t->last);}

global const u8* mkeys;
internal char get_key_pressed(u32 key)
{
    return mkeys[key];
}

global u32 pixels[D_WIDTH * D_HEIGHT];
#define putpixel(x,y,color) pixels[y * D_WIDTH + x] = color;

internal void clear()
{
    memset(pixels, 0, sizeof(u32) * D_WIDTH * D_HEIGHT);
}

void setup()
{
    
}

void render(float dt)
{
    
}

int main(int argc, char ** argv)
{
    
    char quit = false;
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow(
        "Taah",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        D_WIDTH, D_HEIGHT, 0);
    SDL_SetWindowResizable(window, SDL_FALSE);
    
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture * texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STATIC,
        D_WIDTH, D_HEIGHT);
    
    setup();
    Timer frame_timer = {};
    
    
    mkeys = SDL_GetKeyboardState(0);
    while (!quit)
    {
        SDL_UpdateTexture(texture, NULL, pixels, D_WIDTH * sizeof(Uint32));
        SDL_PollEvent(&event);
        
        switch (event.type)
        {
            case SDL_QUIT:
            {
                quit = true;
            } break;
        }
        
        /* Rendering */
        {
            clear();
            render(Timer_get_elapsed_sec(&frame_timer));
            Timer_reset(&frame_timer);
        }
        
        //SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
