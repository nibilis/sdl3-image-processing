#include "image.h"
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL.h>

void MyImage_destroy(MyImage *image)
{
  if (!image)
    return;

  if (image->texture)
  {
    SDL_DestroyTexture(image->texture);
    image->texture = NULL;
  }

  if (image->surface)
  {
    SDL_DestroySurface(image->surface);
    image->surface = NULL;
  }
}

void load_rgba32(const char *filename, SDL_Renderer *renderer, MyImage *output_image)
{
  SDL_Log(">>> load_rgba32(\"%s\")", filename);

  if (!filename)
  {
    SDL_Log("\t*** Erro: Nome do arquivo inválido (filename == NULL).");
    SDL_Log("<<< load_rgba32(\"%s\")", filename);
    return;
  }

  if (!renderer)
  {
    SDL_Log("\t*** Erro: Renderer inválido (renderer == NULL).");
    SDL_Log("<<< load_rgba32(\"%s\")", filename);
    return;
  }

  if (!output_image)
  {
    SDL_Log("\t*** Erro: Imagem de saída inválida (output_image == NULL).");
    SDL_Log("<<< load_rgba32(\"%s\")", filename);
    return;
  }

  MyImage_destroy(output_image);

  SDL_Log("\tCarregando imagem \"%s\" em uma superfície...", filename);
  SDL_Surface *surface = IMG_Load(filename);
  if (!surface)
  {
    SDL_Log("\t*** Erro ao carregar a imagem: %s", SDL_GetError());
    SDL_Log("<<< load_rgba32(\"%s\")", filename);
    return;
  }

  SDL_Log("\tConvertendo superfície para formato RGBA32...");
  output_image->surface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
  SDL_DestroySurface(surface);
  if (!output_image->surface)
  {
    SDL_Log("\t*** Erro ao converter superfície para formato RGBA32: %s", SDL_GetError());
    SDL_Log("<<< load_rgba32(\"%s\")", filename);
    return;
  }

  if(!is_greyscale(output_image->surface))
  {
    to_greyscale(renderer, output_image);
  }

  SDL_Log("\tCriando textura a partir da superfície...");
  output_image->texture = SDL_CreateTextureFromSurface(renderer, output_image->surface);
  if (!output_image->texture)
  {
    SDL_Log("\t*** Erro ao criar textura: %s", SDL_GetError());
    SDL_Log("<<< load_rgba32(\"%s\")", filename);
    return;
  }

  SDL_GetTextureSize(output_image->texture, &output_image->rect.w, &output_image->rect.h);
}

bool is_greyscale(SDL_Surface *surface)
{
  SDL_Log(">>> is_greyscale()");

  if (!surface)
  {
    SDL_Log("\t*** Erro: Superfície inválida (surface == NULL).");
    SDL_Log("<<< is_greyscale()");
    return false;
  }
  if (!surface)
  {
    SDL_Log("\t*** Erro: Superfície inválida (surface == NULL).");
    SDL_Log("<<< is_greyscale()");
    return false;
  }

  const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(surface->format);
  const size_t pixelCount = surface->w * surface->h;

  Uint32 *pixels = (Uint32 *)surface->pixels;
  Uint8 r = 0;
  Uint8 g = 0;
  Uint8 b = 0;
  Uint8 a = 0;

  for (size_t i = 0; i < pixelCount; ++i)
  {
    SDL_GetRGBA(pixels[i], format, NULL, &r, &g, &b, &a);

    if (r != g || r != b)
    {
      SDL_Log("\tImagem não é em tons de cinza.");
      SDL_Log("<<< is_greyscale()");
      return false;
    }
  }

  SDL_Log("\tImagem é em tons de cinza.");
  SDL_Log("<<< is_greyscale()");
  return true;
}

void to_greyscale(SDL_Renderer *renderer, MyImage *image)
{
  SDL_Log(">>> to_greyscale()");

  if (!renderer)
  {
    SDL_Log("\t*** Erro: Renderer inválido (renderer == NULL).");
    SDL_Log("<<< to_greyscale()");
    return;
  }

  if (!image || !image->surface)
  {
    SDL_Log("\t*** Erro: Imagem inválida (image == NULL ou image->surface == NULL).");
    SDL_Log("<<< to_greyscale()");
    return;
  }

  // Para acessar os pixels de uma superfície, precisamos chamar essa função.
  SDL_LockSurface(image->surface);

  const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(image->surface->format);
  const size_t pixelCount = image->surface->w * image->surface->h;

  Uint32 *pixels = (Uint32 *)image->surface->pixels;
  Uint8 r = 0;
  Uint8 g = 0;
  Uint8 b = 0;
  Uint8 a = 0;

  for (size_t i = 0; i < pixelCount; ++i)
  {
    SDL_GetRGBA(pixels[i], format, NULL, &r, &g, &b, &a);

    Uint8 y = 0.2125 * r + 0.7154 * g + 0.0721 * b;

    pixels[i] = SDL_MapRGBA(format, NULL, y, y, y, a);
  }

  SDL_UnlockSurface(image->surface);

  SDL_Log("<<< to_greyscale()");
}