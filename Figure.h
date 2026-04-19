#ifndef FIGURE_H
#define FIGURE_H

#include <SDL2/SDL.h>   
#include "common_types.h"



#define MAX_PLOTS 100

#define FIGURE_MIN_WIDTH 320
#define FIGURE_MIN_HEIGHT 240

typedef struct Figure Figure;

struct Figure{
    SDL_Window* window;
    SDL_Renderer* renderer;
    void* plotting_area;
};


Figure* Figure_Create();
//void Figure_AddPlottingArea(Figure* figure);
void Figure_Update(Figure* figure);
void Figure_Show(Figure* figure);
void Figure_plot(Figure* figure, plot* p);

#endif // FIGURE_H