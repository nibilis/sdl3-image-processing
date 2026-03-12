#ifndef TYPES_H
#define TYPES_H

#include <SDL3/SDL.h>

enum constants
{
  DEFAULT_WINDOW_WIDTH = 640,
  DEFAULT_WINDOW_HEIGHT = 480,
};

typedef struct
{
  SDL_Window *window;
  SDL_Renderer *renderer;
} MyWindow;

typedef struct
{
  SDL_Surface *surface;
  SDL_Texture *texture;
  SDL_FRect rect;
} MyImage;

#endif