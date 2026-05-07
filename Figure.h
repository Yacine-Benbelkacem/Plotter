#ifndef FIGURE_H
#define FIGURE_H

#include <stdint.h>
#include <SDL2/SDL.h>
#include "common_types.h"
#include "Object.h"


#define MAX_PLOTS 100
#define MAX_SUBPLOTS 128

#define FIGURE_MIN_WIDTH 320
#define FIGURE_MIN_HEIGHT 240

typedef struct Figure Figure;

struct Figure{
    SDL_Window* window;
    SDL_Renderer* renderer;
    Object* subplots[MAX_SUBPLOTS];
    int32_t subplot_count;
    char* title[256];
};


Figure* Figure_create(const char* title);
void Figure_get_size(const Figure * figure, int32_t * width, int32_t height);
int32_t Figure_add_subplot(Figure* figure);
void Figure_plot(Figure* self, plot* plt);
void Figure_update(Figure* figure);
void Figure_show(Figure* figure);
void Figure_update_layout(Figure* figure);

#endif // FIGURE_H