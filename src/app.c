#include "app.h"
#include "window.h"
#include "image.h"
#include "types.h"
#include <SDL3/SDL.h>

static const char *WINDOW_TITLE = "Invert image";

static MyWindow g_window = {0};
static MyImage g_image = {0};

static void render(void);

SDL_AppResult initialize(const char *image_filename)
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("Erro SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!MyWindow_initialize(&g_window, WINDOW_TITLE, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0))
  {
      return SDL_APP_FAILURE;
  }

  load_rgba32(image_filename, g_window.renderer, &g_image);

  int imageWidth = (int)g_image.rect.w;
  int imageHeight = (int)g_image.rect.h;

  if (imageWidth > DEFAULT_WINDOW_WIDTH || imageHeight > DEFAULT_WINDOW_HEIGHT)
  {
      int top = 0;
      int left = 0;

      SDL_GetWindowBordersSize(g_window.window, &top, &left, NULL, NULL);

      SDL_SetWindowSize(g_window.window, imageWidth, imageHeight);
      SDL_SetWindowPosition(g_window.window, left, top);

      SDL_SyncWindow(g_window.window);
  }

  return SDL_APP_CONTINUE;
}

void shutdown(void)
{
    SDL_Log(">>> shutdown()");

    MyImage_destroy(&g_image);
    MyWindow_destroy(&g_window);

    SDL_Quit();

    SDL_Log("<<< shutdown()");
}

static void render(void)
{
    SDL_SetRenderDrawColor(g_window.renderer, 128, 128, 128, 255);
    SDL_RenderClear(g_window.renderer);

    SDL_RenderTexture(g_window.renderer, g_image.texture, &g_image.rect, &g_image.rect);

    SDL_RenderPresent(g_window.renderer);
}

void loop(void)
{
  bool mustRefresh = false;
  render();

  SDL_Event event;
  bool isRunning = true;
  while (isRunning)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_EVENT_QUIT:
        isRunning = false;
        break;

      case SDL_EVENT_KEY_DOWN:
        if (event.key.key == SDLK_1 && !event.key.repeat)
        {
          to_greyscale(g_window.renderer, &g_image);
          mustRefresh = true;
        }
        break;
      }
    }

    if (mustRefresh)
    {
      render();
      mustRefresh = false;
    }
  }
}