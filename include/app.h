#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>

SDL_AppResult initialize(const char *image_filename);
void render(void);
void shutdown(void);
void loop(void);
SDL_AppResult initialize_image(const char *image_filename);
SDL_AppResult initialize_app(void);

#endif