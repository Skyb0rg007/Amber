#ifndef AMBER_GRAPHICS_GRAPHICS_SYSTEM_H
#define AMBER_GRAPHICS_GRAPHICS_SYSTEM_H

#include <Amber/util/common.h>
#include <SDL.h>

struct graphics_data {
    SDL_Window *win;
    SDL_GLContext gl_ctx;
    char *base_path;
    char *pref_path;
};

#define AMBER_ORG "sosscorp"
#define AMBER_APP "Amber engine"

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH  700

#endif /* AMBER_GRAPHICS_GRAPHICS_SYSTEM_H */
