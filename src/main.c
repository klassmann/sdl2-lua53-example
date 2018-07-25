//////////////////////////////////////////////////////////////////////////////
// Author: Lucas Klassmann
// License: MIT
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

// Structures
typedef struct AppSettings {
    char *Title;
    int ScreenWidth;
    int ScreenHeight;
    SDL_Color BackgroundColor;
} appsettings_t;

// Constants
// const char * AppTitle = "Example";
// const int ScreenWidth = 800;
// const int ScreenHeight = 600;
const char *SettingsFile = "settings.lua";
const char *ScriptFile = "script.lua";

// Prototypes
void update();
void draw();

void log_error(const char *fmt, ...) {
    va_list valist;
    vfprintf(stderr, fmt, valist);
}

// Global Variables
appsettings_t * gSettings = NULL;
SDL_Window * gWindow = NULL;
SDL_Renderer * gRenderer = NULL;

// API between Lua and SDL
int api_drawline(lua_State *l) {

    int x1 = luaL_checkinteger(l, 1);
    int y1 = luaL_checkinteger(l, 2);
    int x2 = luaL_checkinteger(l, 3);
    int y2 = luaL_checkinteger(l, 4);

    SDL_RenderDrawLine(gRenderer, x1, y1, x2, y2);

    return 0;
}

void setup_api(lua_State *l) {
    lua_pushcfunction(l, api_drawline);
    lua_setglobal(l, "drawline");
}

// Lua Routines
lua_State *open_lua() {
    lua_State *l = luaL_newstate();
    luaL_openlibs(l);
    return l;
}

char * lua_getstring(lua_State *l, const char *name) {
    lua_getglobal(l, name);
    const char * var = lua_tostring(l, -1);
    lua_pop(l, 1);
    char *str = (char *)malloc(strlen(var));
    strcpy(str, var);
    return str;
}

long lua_getint(lua_State *l, const char *name) {
    lua_getglobal(l, name);
    long var = lua_tointeger(l, -1);
    lua_pop(l, 1);
    return var;
}

// Use after calling lua_getglobal
int lua_gettablefield(lua_State *l, const char *name) {
    lua_getfield(l, -1, name);
    if (lua_isstring(l, -1)) {
        int v = lua_tointeger(l, -1);
        lua_pop(l, 1);
        return v;
    }
    return 0;
}

SDL_Color lua_getcolor(lua_State *l, const char *name) {
    SDL_Color color;
    lua_getglobal(l, name);
    if (lua_istable(l, -1)) {
        color.r = lua_gettablefield(l, "r");
        color.g = lua_gettablefield(l, "g");
        color.b = lua_gettablefield(l, "b");
        return color;
    }
    return color;
}

// It opens a configuration file and find for some pre-defined
appsettings_t *load_configuration(const char *filename) {

    lua_State *l = open_lua();

    if (luaL_loadfile(l, filename) == LUA_OK) {
        if (lua_pcall(l, 0, 1, 0) == LUA_OK) {
            lua_pop(l, lua_gettop(l));
        }
    }

    appsettings_t *settings = (appsettings_t*) malloc(sizeof(appsettings_t));

    settings->Title = lua_getstring(l, "app_title");
    settings->ScreenWidth = lua_getint(l, "screen_width");
    settings->ScreenHeight = lua_getint(l, "screen_height");

    lua_close(l);

    return settings;
}

lua_State *load_script(const char *filename) {
    lua_State *l = open_lua();

    setup_api(l);

    if (luaL_loadfile(l, "example5.lua") == LUA_OK) {
        if (lua_pcall(l, 0, 1, 0) == LUA_OK) {
            lua_pop(l, lua_gettop(l));
        }
    }

    return l;
}

void run_method(lua_State *l, const char *method) {
    lua_getglobal(l, method);

    if (lua_isfunction(l, -1)) {
        if (!(lua_pcall(l, 0, 0, 0) == LUA_OK)) {
            log_error("Error on run method: %s\n", method);
        }
    }
}


int main(int argc, char ** argv) {

     // Loading settings from settings.lua
     gSettings = load_configuration(SettingsFile);

    // fprintf(stderr, "Title: %s\n", gSettings->Title);
    // fprintf(stderr, "Screen Width: %d\n", gSettings->ScreenWidth);
    // fprintf(stderr, "Screen Height: %d\n", gSettings->ScreenHeight);

    //  lua_State *mainScript = load_script(ScriptFile);

    if (SDL_Init( SDL_INIT_VIDEO) < 0) {
        log_error("Could not initialize SDL_Init: %s\n", SDL_GetError());
        return -1;
    }
        
    gWindow = SDL_CreateWindow(gSettings->Title, 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        gSettings->ScreenWidth, 
        gSettings->ScreenHeight, 
        SDL_WINDOW_SHOWN );
    
    if (gWindow == NULL) {
        log_error("Could not initialize Window: %s\n", SDL_GetError());
        return -1;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    if (gRenderer == NULL) {
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
        SDL_SetRenderDrawColor(gRenderer, 
            gSettings->BackgroundColor.r, 
            gSettings->BackgroundColor.g, 
            gSettings->BackgroundColor.b, 
            0xFF);
        SDL_RenderClear(gRenderer);

        // Render objects
        draw();
        // run_method(mainScript, "draw");

        // Update screen
        SDL_RenderPresent(gRenderer);
    }

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    // lua_close(mainScript);
    free(gSettings);

    gWindow = NULL;
    gRenderer = NULL;

    SDL_Quit();

    return 0;
}

void update() {
    // Update routines
}

void draw() {
    // Draw routines
}