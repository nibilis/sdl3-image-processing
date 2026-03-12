#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>

SDL_AppResult initialize(const char *image_filename);
void shutdown(void);
void loop(void);

#endif