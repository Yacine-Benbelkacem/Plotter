#ifndef SUBPLOT_H
#define SUBPLOT_H

#include <SDL2/SDL.h>
#include "Object.h"
#include "common_types.h"
#include "Frame.h"
#include "PlotLayer.h"

#define MAX_PLOTS_PER_SUBPLOT 10
#define SUBPLOT_V_MARGIN_RATIO  0.02
#define SUBPLOT_H_MARGIN_RATIO  0.01


typedef struct Subplot{
    Object     base; // Inherit from Object
    SDL_Rect*  viewport;
    PlotLayer* layers[MAX_PLOTS_PER_SUBPLOT];
    int subplot_layers_count;
}Subplot;

Subplot* Subplot_Create();
int Subplot_AddPlot(Subplot* self, plot* p);
void Subplot_Update(Subplot* self);
void Subplot_Render(void* self, void* renderer);
void Subplot_Destroy(void* subplot_ptr);
#endif