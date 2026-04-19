#include "PlottingArea.h"
#include "Figure.h"
#include "PlotLayer.h"


void PlottingArea_Update(PlottingArea* item){
    
    Figure * fig = item->figure;

    SDL_GetWindowSize(fig->window, &item->width, &item->height);
    item->height = item->height- 20;
    item->width  = item->width - 20;

    item->area->x = item->frame.Hom.t.u;
    item->area->y = item->frame.Hom.t.v;
    item->area->w = item->width;
    item->area->h = item->height;

    SDL_SetRenderDrawColor(fig->renderer, 128, 128, 128, 255);
    SDL_RenderFillRect(fig->renderer, item->area);

    SDL_SetRenderDrawColor(fig->renderer, 0, 255, 255, 255);
    SDL_RenderDrawRect(fig->renderer, item->area);


    for(int i = 0; i < item->num_layers; i++){
        if(item->layers[i] != NULL){
            PlotLayer_plot_update(item->layers[i]);
        }
    }
}

PlottingArea*  PlottingArea_Create(Figure* fig){
    PlottingArea* item = (PlottingArea*)malloc(sizeof(PlottingArea));
    item->figure = fig;
    item->frame.Hom.t.u = 10;
    item->frame.Hom.t.v = 10;
    item->frame.Hom.R.rxx =  1; item->frame.Hom.R.rxy =  0;
    item->frame.Hom.R.ryx =  0; item->frame.Hom.R.ryy = -1;
    item->area = (SDL_Rect*)malloc(sizeof(SDL_Rect));

    item->num_layers = 0;
    memset(item->layers, 0, sizeof(item->layers));

    return item;
}