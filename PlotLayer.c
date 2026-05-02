#include "Figure.h"
#include "PlottingArea.h"
#include "PlotLayer.h"
#include "Plot.h"
#include <math.h>

void PlotLayer_render(void* self, void* renderer){
    PlotLayer* layer = (PlotLayer*)self;
    if(layer != NULL){
        PlotLayer_plot_update(layer);
    }
}

int rescale(PlotLayer * layer){
    // Implement rescaling logic here
    // get the max y value
    float max_y = 0;
    float min_y = 0;

    for(int i = 0; i < layer->NbPointsToDisplay; i++){
        if(layer->y_resampledValues[i] > max_y){
            max_y = layer->y_resampledValues[i];
            layer->max_val_idx = i;
        }
    }

    for (int i = 0; i < layer->NbPointsToDisplay; i++)
    {
        if(layer->y_resampledValues[i] < min_y){
            min_y = layer->y_resampledValues[i];
            layer->min_val_idx = i;
        }
    }
    

    for(int i = 0; i < layer->NbPointsToDisplay; i++){
        layer->y_pxlValues[i] = (layer->y_resampledValues[i] / (max_y - min_y)) * layer->area->height;
    }
}

void PlotLayer_plot_data(PlotLayer* layer){
    if(layer != NULL && 
       layer->data != NULL &&
       layer->area != NULL){

        SDL_SetRenderDrawColor(layer->area->figure->renderer, 50, 20, 170, 255);
        for(int i = 0; i < layer->NbPointsToDisplay - 1; i++){
            int pxl_x1 = i * layer->pxl_resolution  + layer->area->origin_x;
            int pxl_y1 = layer->area->origin_y + layer->area->height - layer->y_pxlValues[i] + layer->y_pxlValues[layer->min_val_idx];
            int pxl_x2 = (i + 1) * layer->pxl_resolution + layer->area->origin_x;
            int pxl_y2 = layer->area->origin_y + layer->area->height - layer->y_pxlValues[i + 1] + layer->y_pxlValues[layer->min_val_idx];

            SDL_RenderDrawLine(layer->area->figure->renderer, pxl_x1, pxl_y1, pxl_x2, pxl_y2);
        }
    }
}

void PlotLayer_plot_update(PlotLayer* self){
    if(self != NULL && 
       self->data != NULL){

        int num_data_points = self->data->num_points;
        int current_layer_size = ((Object*)self)->width;

        int NbPointsToDisplay = ( (current_layer_size + 1) / PLOTLAYER_RESOLUTION );

        printf("NbPointsToDisplay: %d\n", NbPointsToDisplay);

        plot_set_nb_points_to_display(self->data, NbPointsToDisplay);


        rescale(self);

        PlotLayer_plot_data(self);
    }
}

void PlotLayer_Destroy(PlotLayer* layer){
    if(layer != NULL){
        if(layer->data != NULL){
            free(layer->data);
        }
        free(layer);
    }
}

PlotLayer* PlotLayer_Create(){
    PlotLayer* layer = (PlotLayer*)malloc(sizeof(PlotLayer));
    if(layer == NULL){
        return NULL;
    }
    layer->data = NULL;
    layer->pxl_resolution = PLOTLAYER_RESOLUTION;
    layer->base.render = PlotLayer_render;
    layer->base.destroy = PlotLayer_Destroy;
    return layer;
}

