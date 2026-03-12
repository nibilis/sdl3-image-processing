#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "types.h"

bool MyWindow_initialize(MyWindow *window, const char *title, int width, int height, SDL_WindowFlags flags);
void MyWindow_destroy(MyWindow *window);

#endif