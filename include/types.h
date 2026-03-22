#ifndef TYPES_H
#define TYPES_H

#include <SDL3/SDL.h>

enum constants
{
  DEFAULT_IMAGE_WINDOW_WIDTH = 640,
  DEFAULT_IMAGE_WINDOW_HEIGHT = 480,
  DEFAULT_APP_WINDOW_WIDTH = 640,
  DEFAULT_APP_WINDOW_HEIGHT = 480,
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

typedef struct {
  SDL_FRect rect;
  SDL_Color color;
  const char* label;
  bool is_pressed;
} Button;

#endif