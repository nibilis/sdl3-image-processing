#ifndef IMAGE_H
#define IMAGE_H

#include <SDL3/SDL.h>
#include "types.h"

void MyImage_destroy(MyImage *image);
void load_rgba32(const char *filename, SDL_Renderer *renderer, MyImage *output_image);
void to_greyscale(SDL_Renderer *renderer, MyImage *image);
bool is_greyscale(SDL_Surface *surface);

#endif