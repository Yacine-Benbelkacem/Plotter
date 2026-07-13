#ifndef SUBPLOT_H
#define SUBPLOT_H

#include <SDL2/SDL.h>
#include "Object.h"
#include "common_types.h"
#include "Frame.h"
#include "PlotLayer.h"

#define MAX_PLOTS_PER_SUBPLOT 10

/* Fixed pixel margins reserved inside the subplot for axis decorations. */
#define AXIS_MARGIN_LEFT    55  /* y tick labels + y-axis title */
#define AXIS_MARGIN_BOTTOM  45  /* x tick labels + x-axis title */
#define AXIS_MARGIN_TOP     32  /* main title */
#define AXIS_MARGIN_RIGHT   18

typedef struct Subplot{
    Object     base; // Inherit from Object
    SDL_Rect*  viewport;
    PlotLayer* layers[MAX_PLOTS_PER_SUBPLOT];
    int        subplot_layers_count;
    char       title[128];
    char       x_label[64];
    char       y_label[64];
    SDL_Rect   plot_area;   // data-drawing rectangle, in window coordinates
}Subplot;

Subplot* Subplot_Create();
int  Subplot_AddPlot(Subplot* self, plot* p);
void Subplot_Update(Subplot* self);
void Subplot_Render(void* self, void* renderer);
void Subplot_Destroy(void* subplot_ptr);

void Subplot_SetTitle(Subplot* self, const char* title);
void Subplot_SetXLabel(Subplot* self, const char* label);
void Subplot_SetYLabel(Subplot* self, const char* label);

/* If (mx,my) in window coordinates lies within the plot area, zoom every
   layer about that point by `factor` and return 1; otherwise return 0. */
int  Subplot_ZoomAt(Subplot* self, int mx, int my, double factor);

#endif
