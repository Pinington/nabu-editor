#ifndef EDITOR_H
#define EDITOR_H

#include <SDL2/SDL.h>

#define WINDOW_TITLE "Text Editor"
#define FPS 25

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 256

#define FONT_SIZE 18
#define BUFFER_SIZE 256
#define MAX_LINES 256

struct Display {
    SDL_Window* window;
    SDL_Renderer* renderer;
};

void start_console();

#endif