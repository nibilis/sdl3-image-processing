#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>

SDL_AppResult initialize(const char *image_filename);
void render(void);
void shutdown(void);
void loop(void);
SDL_AppResult initialize_image(const char *image_filename);
SDL_AppResult initialize_app(void);
void save_image(void);
void drawHistogram(float histogram[256]);
void equalizeHistogram(float histogram[256]);
void applyEqualization();
void restoreOriginal();
float getMean(float histogram[256]);
const char* classifyBrightness(float mean);
float computeStdDev(float hist[256], float mean);
const char* classifyContrast(float stddev);
void printImageInfo(float histogram[256]);

#endif