#include "app.h"
#include "window.h"
#include "image.h"
#include "types.h"
#include "button.h"
#include <SDL3/SDL.h>
#include <math.h>

static const char *IMAGE_WINDOW_TITLE = "Projeto 1 - Imagem";
static const char *APP_WINDOW_TITLE = "Projeto 1 - App";

static MyWindow image_window = {0};
static MyWindow app_window = {0};
static MyImage g_image = {0};

static float histogram[256] = {0};
static bool equalized = false;
static int CDFValues[256];
static float equalizedHistogram[256] = {0};

Button btnEqualize = {{50, 300, 150, 40}, {100, 100, 100, 255}, "Equalize", false};
Button btnClose = {{220, 300, 100, 40}, {150, 50, 50, 255}, "Close", false};
Button btnSave = {{390, 300, 100, 40}, {50, 50, 200, 255}, "Save", false};

static SDL_Surface *original_grayscale_surface = NULL;

SDL_AppResult initialize(const char *image_filename)
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("Erro SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  initialize_image(image_filename);
  original_grayscale_surface = SDL_DuplicateSurface(g_image.surface);
  // SDL_Log("### image initialized ###");
  
  
  initialize_app();
  //SDL_Log("### app initialized ###");  

  return SDL_APP_CONTINUE;
}

void shutdown(void)
{
  // SDL_Log(">>> shutdown()");

  MyImage_destroy(&g_image);
   // SDL_Log("Image Destroyed");
  MyWindow_destroy(&image_window);
   // SDL_Log("image window Destroyed");
  MyWindow_destroy(&app_window);
   // SDL_Log("app window destroyed");

  SDL_Quit();

  // SDL_Log("<<< shutdown()");
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
  }

  SDL_UnlockSurface(g_image.surface);

  printImageInfo(histogram);

  return SDL_APP_CONTINUE;

}

void save_image()
{
  const char *filename = "output_image.png";

  bool saved = SDL_SavePNG(g_image.surface, filename);

  if (!saved)
  {
    SDL_Log("Erro ao salvar a imagem");
  }

  SDL_Log("Imagem salva em %s", filename);
}

void drawHistogram(float histogram[256]){
  float max = 0;

  for (int i = 0; i < 256; i++) {
      if (histogram[i] > max) max = histogram[i];
  }

  int width = DEFAULT_APP_WINDOW_WIDTH;
  int height = DEFAULT_APP_WINDOW_HEIGHT;

  if (max == 0){
    return;
  }

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
  // funcao transformacao
  float value = 0.0f;
  for (int i = 0; i < 256; i++){
    value += histogram[i];
    int s = (int)(255.0f * value + 0.5f); // arredonda pra cima
    if (s > 255) {s = 255;}
    CDFValues[i] = s;
  }

  for (int i = 0; i < 256; i++){
    equalizedHistogram[i] = 0.0f;
  }

  for(int i = 0 ; i < 256; i++){
    int new_val = CDFValues[i];
    equalizedHistogram[new_val] += histogram[i];
  }
}

void applyEqualization(){
  SDL_LockSurface(g_image.surface);

  const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(g_image.surface->format);

  const SDL_Palette *palette = SDL_GetSurfacePalette(g_image.surface);

  Uint32 *pixels = (Uint32 *)g_image.surface->pixels;

  int total = g_image.surface->w * g_image.surface->h;

  for(int i = 0; i < total; i++){
    Uint8 r,g,b,a;

    SDL_GetRGBA(pixels[i], format, palette, &r, &g, &b, &a);

    Uint8 newGray = CDFValues[r];

    pixels[i] = SDL_MapRGBA(format, palette, newGray, newGray, newGray, a);
  }

  SDL_UnlockSurface(g_image.surface);

  SDL_UpdateTexture(g_image.texture, NULL, g_image.surface->pixels, g_image.surface->pitch);

}

void restoreOriginal(){
  if(!original_grayscale_surface) return;

  SDL_BlitSurface(original_grayscale_surface, NULL, g_image.surface, NULL);

  SDL_UpdateTexture(g_image.texture, NULL, g_image.surface->pixels, g_image.surface->pitch);

}

float getMean(float histogram[256]){
  float mean = 0.0f;

  for (int i = 0; i < 256; i++){
    mean += i * histogram[i];
  }

  return mean;
}

const char* classifyBrightness(float mean){
    if(mean < 85) return "Escura";
    else if(mean < 170) return "Media";
    else return "Clara";
}

float computeStdDev(float hist[256], float mean){
    float variance = 0.0f;

    for(int i = 0; i < 256; i++){
        float diff = i - mean;
        variance += diff * diff * hist[i];
    }

    return sqrtf(variance);
}

const char* classifyContrast(float stddev){
    if(stddev < 40) return "Baixo";
    else if(stddev < 80) return "Medio";
    else return "Alto";
}

void printImageInfo(float histogram[256]){
    float mean = getMean(histogram);
    float stddev = computeStdDev(histogram, mean);

    SDL_Log("Brilho: %s (%.2f)", classifyBrightness(mean), mean);

    SDL_Log("Contraste: %s (%.2f)", classifyContrast(stddev), stddev);
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
    draw_button(app_window.renderer, btnSave);

    if(equalized){
      drawHistogram(equalizedHistogram);
      btnEqualize.label = "Normalize";
    } else{
      drawHistogram(histogram);
      btnEqualize.label = "Equalize";
    }

    

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
          if(!equalized){
            SDL_Log("Equalizando histograma...\n");
            equalizeHistogram(histogram);
            SDL_Log("Histograma equalizado\n");
            SDL_Log("Aplicando equalização à imagem...");
            applyEqualization();
            SDL_Log("Imagem equalizada\n");
            render();
            printImageInfo(equalizedHistogram);
          } else{
            SDL_Log("Revertendo a equalização da imagem...\n");
            restoreOriginal();
            render();
            SDL_Log("Imagem grayscale original restaurada");
            printImageInfo(histogram);
          }
          equalized = !equalized;
        }

        if (is_point_in_rect(mouseX, mouseY, btnClose.rect) && mousepressed) {
          isRunning = false; 
        }

        if (is_point_in_rect(mouseX, mouseY, btnSave.rect) && mousepressed) {
          SDL_Log("Salvando imagem...\n");
          save_image();
        }
      }
    }

    if (mustRefresh)
    {
      render();
      mustRefresh = false;
    }
  }
  
  // SDL_Log("<<< loop()");
}