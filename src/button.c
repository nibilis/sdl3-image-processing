#include <SDL3/SDL.h>
#include "types.h"

void draw_button(SDL_Renderer* renderer, Button btn) {
  SDL_SetRenderDrawColor(renderer, btn.color.r, btn.color.g, btn.color.b, btn.color.a);
  SDL_RenderFillRect(renderer, &btn.rect);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderRect(renderer, &btn.rect);

  SDL_RenderDebugText(renderer, btn.rect.x + 10, btn.rect.y + 12, btn.label);
}

bool is_point_in_rect(float x, float y, SDL_FRect rect) {
  return (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h);
}