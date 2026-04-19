#ifndef PLOTTINGAREA_H
#define PLOTTINGAREA_H  

#include "common_types.h"
#include "Frame.h"

#include <SDL2/SDL.h>   


#define MAX_PLOTS 100

struct PlottingArea{
    Figure* figure;
    Frame frame;
    int width;
    int height;
    PlotLayer* layers[MAX_PLOTS];
    int num_layers;
    SDL_Rect* area;
};

void PlottingArea_Update(PlottingArea* item);
PlottingArea*  PlottingArea_Create(Figure* fig);


#endif // PLOTTINGAREA_H