#if __INTELLISENSE__
typedef unsigned int __SIZE_TYPE__;
typedef unsigned long __PTRDIFF_TYPE__;
#define __attribute__(q)
#define __builtin_strcmp(a,b) 0
#define __builtin_strlen(a) 0
#define __builtin_memcpy(a,b) 0
#define __builtin_va_list void*
#define __builtin_va_start(a,b)
#define __extension__
#endif

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif


// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>
#include <SDL2/SDL.h>

// Main program entrypoint
//Using SDL and standard IO

//Screen dimension constants
const double Pi = 3.14159265358979323846264;
static int iterations = 15;
static int size = 2;

double Convert(float degree)
{
    return (degree * (Pi / 180));
}

void drawFractal(SDL_Renderer* renderer, Uint16 x1, Uint16 y1, float angle, int depth, Uint8 size, float factor) {
    if (depth > 0) {
        Uint16 x2 = x1 + cos(Convert(angle)) * depth * size * factor;
        Uint16 y2 = y1 + sin(Convert(angle)) * depth * size * factor;
        Uint8 color = rand() % 255;
        if (depth > iterations / 2) {
            SDL_SetRenderDrawColor(renderer, color, 0, 0, SDL_ALPHA_OPAQUE);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 0, color, 0, SDL_ALPHA_OPAQUE);
        }
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        drawFractal(renderer, x2, y2, angle, depth - 2, size, factor);
        drawFractal(renderer, x2, y2, angle - 35, depth - 2, size, factor);
        drawFractal(renderer, x2, y2, angle + 35, depth - 2, size, factor);
    }
}
int main(int argc, char* args[])
{
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        if (SDL_CreateWindowAndRenderer(1280, 720, 0, &window, &renderer) == 0) {
            SDL_bool done = SDL_FALSE;
            int x = 0;
            SDL_SetWindowTitle(window, "Fractal Demo");
            SDL_InitSubSystem(SDL_INIT_JOYSTICK);
            SDL_JoystickEventState(SDL_ENABLE);
            SDL_JoystickOpen(0);
            while (!done) {
                SDL_Event event;

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);
                drawFractal(renderer, 640, 700, -90.0f, iterations, size, 1.0f);
                printf("%i\n", iterations);
                SDL_RenderPresent(renderer);
                x++;
                while (SDL_PollEvent(&event)) {
                    switch (event.type) {
                    case SDL_QUIT:
                        done = SDL_TRUE;
                        break;
                    case SDL_JOYBUTTONDOWN:
                        switch (event.jbutton.button) {
                        case 14:
                            iterations--;
                            break;

                        case 12:
                            iterations++;
                            break;
                        case 13:
                            size--;
                            break;

                        case 15:
                            size++;
                            break;
                        case 10:
                            done = SDL_TRUE;
                            break;
                        }
                    }

                }
            }
        }

        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
    }
    SDL_Quit();
    return 0;
}