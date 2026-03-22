#include "app.h"
#include "window.h"
#include "image.h"
#include "types.h"
#include "button.h"
#include <SDL3/SDL.h>

static const char *IMAGE_WINDOW_TITLE = "Projeto 1 - Imagem";
static const char *APP_WINDOW_TITLE = "Projeto 1 - App";

static MyWindow image_window = {0};
static MyWindow app_window = {0};
static MyImage g_image = {0};

Button btnEqualize = {{50, 300, 150, 40}, {100, 100, 100, 255}, "Equalize", false};
Button btnClose = {{220, 300, 100, 40}, {150, 50, 50, 255}, "Close", false};

SDL_AppResult initialize(const char *image_filename)
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("Erro SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  initialize_image(image_filename);
  initialize_app();  

  return SDL_APP_CONTINUE;
}

void shutdown(void)
{
  SDL_Log(">>> shutdown()");

  MyImage_destroy(&g_image);
  MyWindow_destroy(&image_window);
  MyWindow_destroy(&app_window);

  SDL_Quit();

  SDL_Log("<<< shutdown()");
}

void render(void)
{
  SDL_SetRenderDrawColor(image_window.renderer, 128, 128, 128, 255);
  SDL_RenderClear(image_window.renderer);

  SDL_RenderTexture(image_window.renderer, g_image.texture, &g_image.rect, &g_image.rect);

  SDL_RenderPresent(image_window.renderer);
}

SDL_AppResult initialize_image(const char *image_filename)
{
  if (!MyWindow_initialize(&image_window, IMAGE_WINDOW_TITLE, DEFAULT_IMAGE_WINDOW_WIDTH, DEFAULT_IMAGE_WINDOW_HEIGHT, 0))
  {
    return SDL_APP_FAILURE;
  }

  load_rgba32(image_filename, image_window.renderer, &g_image);

  int imageWidth = (int)g_image.rect.w;
  int imageHeight = (int)g_image.rect.h;

  if (imageWidth > DEFAULT_IMAGE_WINDOW_WIDTH || imageHeight > DEFAULT_IMAGE_WINDOW_HEIGHT)
  {
    int top = 0;
    int left = 0;

    SDL_GetWindowBordersSize(image_window.window, &top, &left, NULL, NULL);

    SDL_SetWindowSize(image_window.window, imageWidth, imageHeight);
    SDL_SetWindowPosition(image_window.window, left, top);

    SDL_SyncWindow(image_window.window);
  }
}

SDL_AppResult initialize_app(void)
{
  if (!MyWindow_initialize(&app_window, APP_WINDOW_TITLE, DEFAULT_APP_WINDOW_WIDTH, DEFAULT_APP_WINDOW_HEIGHT, 0))
  {
    return SDL_APP_FAILURE;
  }
}

void loop(void)
{
  bool mustRefresh = false;
  render();

  SDL_Event event;
  bool isRunning = true;
  while (isRunning)
  {
    SDL_SetRenderDrawColor(app_window.renderer, 33, 33, 33, 255);
    SDL_RenderClear(app_window.renderer);

    draw_button(app_window.renderer, btnEqualize);
    draw_button(app_window.renderer, btnClose);

    SDL_RenderPresent(app_window.renderer);
    
    while (SDL_PollEvent(&event))
    {
      if (event.button.windowID == SDL_GetWindowID(app_window.window)) {
        float mouseX = event.button.x;
        float mouseY = event.button.y;

        if (is_point_in_rect(mouseX, mouseY, btnEqualize.rect)) {
          // printf("Equalizing Histogram...\n");
        }

        if (is_point_in_rect(mouseX, mouseY, btnClose.rect)) {
          isRunning = false; 
        }
      }
      switch (event.type)
      {
      case SDL_EVENT_QUIT:
        isRunning = false;
        break;

      case SDL_EVENT_KEY_DOWN:
        if (event.key.key == SDLK_1 && !event.key.repeat)
        {
          to_greyscale(image_window.renderer, &g_image);
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