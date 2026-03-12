#include "window.h"
#include <SDL3/SDL.h>

bool MyWindow_initialize(MyWindow *window, const char *title, int width, int height, SDL_WindowFlags flags)
{
  if (!window)
    return false;

  return SDL_CreateWindowAndRenderer(title, width, height, flags, &window->window, &window->renderer);
}

void MyWindow_destroy(MyWindow *window)
{
  if (!window)
    return;

  SDL_DestroyRenderer(window->renderer);
  SDL_DestroyWindow(window->window);

  window->renderer = NULL;
  window->window = NULL;
}