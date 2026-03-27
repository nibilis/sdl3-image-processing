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

static float histogram[256] = {0};

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
  SDL_Log("### image initialized ###");
  
  
  initialize_app();
  //SDL_Log("### app initialized ###");  

  return SDL_APP_CONTINUE;
}

void shutdown(void)
{
  SDL_Log(">>> shutdown()");

  MyImage_destroy(&g_image);
   SDL_Log("Image Destroyed");
  MyWindow_destroy(&image_window);
   SDL_Log("image window Destroyed");
  MyWindow_destroy(&app_window);
   SDL_Log("app window destroyed");

  SDL_Quit();

  SDL_Log("<<< shutdown()");
}

void render(void)
{
  SDL_SetRenderDrawColor(image_window.renderer, 128, 128, 128, 255); // define cor
  SDL_RenderClear(image_window.renderer); // "limpa tudo com essa cor"

  SDL_RenderTexture(image_window.renderer, g_image.texture, &g_image.rect, &g_image.rect);

  SDL_RenderPresent(image_window.renderer); // apresenta o conteudo na janela -> conteudo eh o ponteiro sendo passado

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

  SDL_Log("ImageWidth = %i || imageHeight = %i", imageWidth, imageHeight);

  if (imageWidth > DEFAULT_IMAGE_WINDOW_WIDTH || imageHeight > DEFAULT_IMAGE_WINDOW_HEIGHT)
  {
    int top = 0;
    int left = 0;

    SDL_GetWindowBordersSize(image_window.window, &top, &left, NULL, NULL);

    SDL_SetWindowSize(image_window.window, imageWidth, imageHeight);

    SDL_SetWindowPosition(image_window.window, left, top);

    SDL_SyncWindow(image_window.window);
  }

  return SDL_APP_CONTINUE;
  
}

SDL_AppResult initialize_app(void)
{
  if (!MyWindow_initialize(&app_window, APP_WINDOW_TITLE, DEFAULT_APP_WINDOW_WIDTH, DEFAULT_APP_WINDOW_HEIGHT, 0))
  {
    return SDL_APP_FAILURE;
  }

  // passar por cada pixel na imagem
  SDL_LockSurface(g_image.surface);

  const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(g_image.surface->format);
  const size_t pixelCount = g_image.surface->w * g_image.surface->h;

  Uint32 *pixels = (Uint32 *) g_image.surface->pixels;

  Uint8 gray = 0;

 

  for(size_t i = 0; i < pixelCount; i++){
    SDL_GetRGBA(pixels[i], format, NULL, &gray, NULL, NULL, NULL);
    histogram[gray]++;
  }

  // Normalizacao do histograma
    for(int i = 0; i < 256; i++){
    histogram[i] = (histogram[i] / pixelCount);
    SDL_Log("%f", histogram[i]);
  }

 



  return SDL_APP_CONTINUE;

}

void drawHistogram(float histogram[256]){
  float max = 0;

  for (int i = 0; i < 256; i++) {
      if (histogram[i] > max) max = histogram[i];
  }

  int width = DEFAULT_APP_WINDOW_WIDTH;
  int height = DEFAULT_APP_WINDOW_HEIGHT;

  for (int i = 0; i < 256; i++) {
    int x = (i * width) / 256;
    float barHeight = (histogram[i] * height) / max;

    SDL_SetRenderDrawColor(app_window.renderer, 255, 255, 255, 255);

    SDL_RenderLine(
        app_window.renderer,
        x, height,
        x, height - barHeight
    );
  }

}

void equalizeHistogram(float histogram[256]){
  int CDFValues[256]; // (primeira parte do processo de equalizacao - Valores arredondados)
  // funcao transformacao
  float value = 0.0f;
  for (int i = 0; i < 256; i++){
    value += histogram[i];
    CDFValues[i] = (int)(255 * value *0.5f); // arredonda pra cima
    if (CDFValues[i] > 255) {CDFValues[i] = 255;}
  }

  float histogram_equalized[256] = {0};
  for(int i = 0 ; i < 256; i++){
    int new_val = CDFValues[i];
    histogram_equalized[new_val] += histogram[i];
  }

  histogram = histogram_equalized;
  
}

void loop(void)
{
  SDL_Log(">>> loop()");

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

    drawHistogram(histogram);

    SDL_RenderPresent(app_window.renderer);

    while (SDL_PollEvent(&event))
    {
        if (event.button.windowID == SDL_GetWindowID(app_window.window)) {
        float mouseX = event.button.x;
        float mouseY = event.button.y;

        bool mousepressed = false;

        if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){ // necessario para evitar clique duplo
          if (event.button.button == SDL_BUTTON_LEFT){
            mousepressed = true;
          }
        }

        if (is_point_in_rect(mouseX, mouseY, btnEqualize.rect) && mousepressed) {
          equalizeHistogram(histogram);
          SDL_Log("Equalizing Histogram...\n");
        }

        if (is_point_in_rect(mouseX, mouseY, btnClose.rect) && mousepressed) {
          isRunning = false; 
        }
      }
    }

    if (mustRefresh)
    {
      render();
      mustRefresh = false;
    }
  }
  
  SDL_Log("<<< loop()");
}