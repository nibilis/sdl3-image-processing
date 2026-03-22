#ifndef BUTTON_H
#define BUTTON_H

#include <SDL3/SDL.h>

void draw_button(SDL_Renderer* renderer, Button btn);
bool is_point_in_rect(float x, float y, SDL_FRect rect);

#endif