#ifndef SUBPLOT_H
#define SUBPLOT_H

#include <SDL2/SDL.h>
#include "Object.h"
#include "common_types.h"
#include "Frame.h"

#define MAX_PLOTS_PER_SUBPLOT 10

typedef struct Subplot{
    Object     base; // Inherit from Object
    SDL_Rect*  viewport;
    PlotLayer* layers[MAX_PLOTS_PER_SUBPLOT];
    int subplot_layers_count;
}Subplot;

Subplot* Subplot_Create();
void Subplot_Render(void* self, void* renderer);
void Subplot_Destroy(void* subplot_ptr);
#endif