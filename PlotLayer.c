#include "Figure.h"
#include "PlottingArea.h"
#include "PlotLayer.h"
#include "Plot.h"
#include <math.h>


void resample(PlotLayer * layer, int num_points){
    // Implement resampling logic here
    for(int i = 0; i < num_points; i++){
        int index = (i * layer->data->num_points) / num_points;
        layer->x_resampledValues[i] = layer->data->points[index].x;
        layer->y_resampledValues[i] = layer->data->points[index].y;
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
            int pxl_x1 = i * layer->pxl_resolution  + layer->area->frame.Hom.t.u;
            int pxl_y1 = layer->area->frame.Hom.t.v + layer->area->height - layer->y_pxlValues[i] + layer->y_pxlValues[layer->min_val_idx];
            int pxl_x2 = (i + 1) * layer->pxl_resolution + layer->area->frame.Hom.t.u;
            int pxl_y2 = layer->area->frame.Hom.t.v + layer->area->height - layer->y_pxlValues[i + 1] + layer->y_pxlValues[layer->min_val_idx];

            SDL_RenderDrawLine(layer->area->figure->renderer, pxl_x1, pxl_y1, pxl_x2, pxl_y2);
        }
    }
}

void PlotLayer_plot_update(PlotLayer* layer){
    if(layer != NULL && 
       layer->data != NULL &&
       layer->area != NULL){

        int num_data_points = layer->data->num_points;
        int current_layer_size = layer->area->width;

        layer->NbPointsToDisplay = ( (current_layer_size + 1) / PLOTLAYER_RESOLUTION );

        printf("NbPointsToDisplay: %d\n", layer->NbPointsToDisplay);
        if( layer->NbPointsToDisplay < num_data_points){
            resample(layer, layer->NbPointsToDisplay);  
        }else{
            for(int i = 0; i < num_data_points; i++){
                layer->x_resampledValues[i] = layer->data->points[i].x;
                layer->y_resampledValues[i] = layer->data->points[i].y;
            }
            layer->NbPointsToDisplay = num_data_points;
            layer->pxl_resolution = (current_layer_size + 1) / num_data_points;
            printf("pxl_resolution: %d\n", layer->pxl_resolution);
            printf("current_layer_size: %d\n", current_layer_size);
            printf("num_data_points: %d\n", num_data_points);

        }
        rescale(layer);

        PlotLayer_plot_data(layer);
    }
}

PlotLayer* PlotLayer_Create(plot* data, PlottingArea* area){
    PlotLayer* layer = (PlotLayer*)malloc(sizeof(PlotLayer));
    if(layer){
        area->layers[area->num_layers++] = layer;
        layer->data = data;
        layer->area = area;
    }
    layer->pxl_resolution = PLOTLAYER_RESOLUTION;
    return layer;
}
