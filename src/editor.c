#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "editor.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int sdl_initialize(struct Display *display) {
    // Initializes SDL video
    if (SDL_Init(SDL_INIT_VIDEO)) { 
        fprintf(stderr, "SDL Video initialization error: %s\n", SDL_GetError());
        return 1;
    }

    // Initializes the TTF text module thing
    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL TTF (text module) intialization error: %s\n", TTF_GetError());
        return 1;
    }

    // Create Window
    Uint32 window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;

    display->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags);
    if (!display->window) {
        fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
        return 1;
    }

    // Create renderer
    display->renderer = SDL_CreateRenderer(display->window, -1,  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!display->renderer) {
        fprintf(stderr, "Error creating SDL renderer: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}


void display_cleanup(struct Display *display, int exit_status) {
    SDL_DestroyRenderer(display->renderer);
    SDL_DestroyWindow(display->window);
    TTF_Quit();
    SDL_Quit();
}


int get_column_from_x(TTF_Font *font, const char *text, int x) {
    int width = 0;
    int col = 0;
    int cw = 0;

    // The for loop stopping at the last character insures we get 
    // the last char's position if we click in empty space to the right
    for (int i = 0; text[i] != '\0'; i++) {
        // Measure next character
        char temp[2] = { text[i], '\0' };
        TTF_SizeText(font, temp, &cw, NULL);

        // If x falls inside this character, return this col
        if (x < width + cw / 2) return col;

        width += cw;
        col++;
    }

    return col;
}


int text_too_wide(TTF_Font *font, const char *txt) {
    int w = 0;
    TTF_SizeText(font, txt, &w, NULL);
    return w > WINDOW_WIDTH - FONT_SIZE; // Enough space at the end for one character
}


void render_line(struct Display* display, int row, char* txt, SDL_Color* color, TTF_Font** font) {
    // Create the surface of the message
    SDL_Surface* surface_message =
        TTF_RenderText_Blended(*font, txt, *color); 

    // Make surface into message
    SDL_Texture* Message = SDL_CreateTextureFromSurface(display->renderer, surface_message);

    // Create the rectangle that will contain the text and position it
    SDL_Rect Message_rect; 
    Message_rect.x = 10; 
    Message_rect.y = 10 + row * TTF_FontLineSkip(*font);
    SDL_QueryTexture(Message, NULL, NULL, &Message_rect.w, &Message_rect.h);

    // Pass the restangle and the message to the renderer
    SDL_RenderCopy(display->renderer, Message, NULL, &Message_rect);

    // Free all the useless stuff
    SDL_FreeSurface(surface_message);
    SDL_DestroyTexture(Message);
}


void handle_new_line
(int* cursor_row, int* lines, int buffer_size[MAX_LINES], char buffer[MAX_LINES][BUFFER_SIZE]) {
    // Create an empty line below current line and displace all the ones below (linked lists could''ve been useful fr)
    (*cursor_row)++;
    (*lines)++;

    for (int i = *lines; i > *cursor_row; i--) {
        for (int j = 0; j < buffer_size[i] + 1; j++) {
            strcpy(buffer[i], buffer[i - 1]);
            buffer_size[i] = buffer_size[i - 1];
        }
    }

    buffer[*cursor_row][0] = '\0';
    buffer_size[*cursor_row] = 0;
}


void start_console() {
    struct Display display = {.window = NULL, .renderer = NULL};
    if (sdl_initialize(&display)) {
        fprintf(stderr, "Failed to initialize SDL\n");
        return;
    }

    // Select the font
    TTF_Font* Font = TTF_OpenFont("C:/Windows/Fonts/Calibri.ttf", FONT_SIZE);
    if (!Font) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        display_cleanup(&display, EXIT_FAILURE);
        return;
    }

    // Select font color
    SDL_Color Color = {255, 255, 255};

    // Create the text buffers
    char buffer[MAX_LINES][BUFFER_SIZE];
    for (int i = 0; i < 16; i++) buffer[i][0] = '\0';
    int buffer_size[MAX_LINES] = {0};

    // Initalize the cursor position to be on the first line
    int cursor_col = 0;
    int cursor_row = 0;
    int lines = 0;

    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;

                // Handle mouse clicks
                case SDL_MOUSEBUTTONDOWN:
                    int row = (event.button.y - 10) / TTF_FontLineSkip(Font);
                    int col = get_column_from_x(Font, buffer[row], event.button.x - 10);
                    if (row <= lines && col < BUFFER_SIZE) {
                        cursor_row = row;
                        cursor_col = col;
                    } else {
                        // If I click in the empty space below text, set back cursor to last character in my text
                        // Might cause errors if somehow I get row < 0 or something
                        cursor_row = lines;
                        cursor_col = strlen(buffer[cursor_row]);
                    }
                    break;


                // Handle sepcial key presses
                case SDL_KEYDOWN:
                    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                        running = 0;

                    } else if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
                        if (cursor_col > 0) {
                            // If line isnt empty remove char from array
                            for (int i = cursor_col - 1; i < buffer_size[cursor_row]; i++) {
                                buffer[cursor_row][i] = buffer[cursor_row][i + 1];
                            }
                            buffer_size[cursor_row]--;
                            cursor_col--;
                        } else if (cursor_row > 0) {
                            // If line is empty, and it's not the first line go the line above
                            cursor_row--;
                            cursor_col = --buffer_size[cursor_row];
                            buffer[cursor_row][cursor_col] = '\0';
                        }
                        
                    } else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                        if (lines < MAX_LINES - 1) {
                            handle_new_line(&cursor_row, &lines, buffer_size, buffer);
                            cursor_col = 0;
                        }
                    } 
                    break;


                // Handle text input
                case SDL_TEXTINPUT:
                    for (int i = 0; event.text.text[i]; i++) {
                        buffer[cursor_row][cursor_col++] = event.text.text[i];
                        buffer[cursor_row][++buffer_size[cursor_row]] = '\0';

                        if (text_too_wide(Font, buffer[cursor_row]) && lines < MAX_LINES - 1) {

                            // Save last character and then set it to null terminator in the buffer
                            char last = buffer[cursor_row][cursor_col - 1];
                            buffer_size[cursor_row]--;
                            buffer[cursor_row][buffer_size[cursor_row]] = '\0';
                            
                            handle_new_line(&cursor_row, &lines, buffer_size, buffer);

                            // Set first charcter of the new line to be the one saved
                            buffer_size[cursor_row] = 1;
                            buffer[cursor_row][0] = last;
                            buffer[cursor_row][1] = '\0';

                            cursor_col = 1;
                        }
                    }
                    break; 
                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(display.renderer, 32, 32, 32, 255);
        SDL_RenderClear(display.renderer);

        // TTF text section
        
        for (int i = 0; i <= lines; i++) {
            // I'm gonna try and make it more efficient by creating big messages
            render_line(&display, i, buffer[i], &Color, &Font);
        }

        // End of TTF section

        SDL_RenderPresent(display.renderer);
        SDL_Delay(1000 / FPS);
    }
    
    display_cleanup(&display, EXIT_SUCCESS);
}




/* Console version of the text editor works when ran from Admin cmd, but 
has bugs in normal consoles due to windows forcing QuickEdit *

#include <windows.h>
void start_console_prim() {
    FreeConsole(); 
    AllocConsole();

    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);

    HANDLE h_out = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE h_in = GetStdHandle(STD_INPUT_HANDLE);

    SetConsoleMode(h_in, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);

    INPUT_RECORD rec;
    DWORD num_read;
    while (1) { 
        ReadConsoleInput(h_in, &rec, 1, &num_read);

        if (rec.EventType == KEY_EVENT) {
            KEY_EVENT_RECORD key = rec.Event.KeyEvent;

            if (key.bKeyDown) {
                char c = key.uChar.AsciiChar;
                if (c == 'q' - 96) break;
                if (c) putchar(c);
                if (c == '\r') putchar('\n');
            }
        } else if (rec.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD click = rec.Event.MouseEvent;

            if (click.dwEventFlags == 0 && (click.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))  {
                SetConsoleCursorPosition(h_out, click.dwMousePosition);
            }
        }
    }
}
*/
