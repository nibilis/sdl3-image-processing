#include "app.h"
#include <SDL3/SDL.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  atexit(shutdown);
  
  if (argc != 2)
  {
    SDL_Log("Uso: %s <arquivo_de_imagem>", argv[0]);
    return 1;
  }

  char *image_filename = argv[1];

 

  if (initialize(image_filename) == SDL_APP_FAILURE)
    return SDL_APP_FAILURE;

  loop();

  return 0;
}