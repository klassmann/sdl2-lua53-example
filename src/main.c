#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>


void update();
void draw(SDL_Renderer *r);

void log_error(const char *fmt, ...) {
    va_list valist;
    vfprintf(stderr, fmt, valist);
}

const char * AppTitle = "Example";
const int ScreenWidth = 800;
const int ScreenHeight = 600;

int main(int argc, char ** argv) {
    
    SDL_Window * _window = NULL;
    SDL_Renderer * _renderer = NULL;
    if (SDL_Init( SDL_INIT_VIDEO) < 0) {
        log_error("Could not initialize SDL_Init: %s\n", SDL_GetError());
        return -1;
    }
        
    _window = SDL_CreateWindow(AppTitle, 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        ScreenWidth, 
        ScreenHeight, 
        SDL_WINDOW_SHOWN );
    
    if (_window == NULL) {
        log_error("Could not initialize Window: %s\n", SDL_GetError());
        return -1;
    }

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED );

    if (_renderer == NULL) {
        log_error("Could not initialize Renderer: %s\n", SDL_GetError());
        return -1;
    }

    int quit = 0;
    SDL_Event e;

    while (quit != 1) {
        // Events
        while(SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Update inputs
        update();

        // Clear screen
        SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(_renderer);

        // Render objects
        draw(_renderer);

        // Update screen
        SDL_RenderPresent(_renderer);
    }

    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);

    _window = NULL;
    _renderer = NULL;

    SDL_Quit();

    return 0;
}

void update() {
    // Update routines
}

void draw(SDL_Renderer * r) {
    // Draw routines
}