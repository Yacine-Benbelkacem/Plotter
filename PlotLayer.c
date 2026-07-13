#include "Figure.h"
#include "Plot.h"
#include "PlotLayer.h"
#include "Frame.h"
#include <math.h>


static void PlotLayer_plot_data(PlotLayer* layer, SDL_Renderer* renderer){
    if(layer != NULL && layer->data != NULL){
        /* The subplot draws the axis frame; the layer only draws the trace. */
        ((Object*)layer->data)->render((Object*)layer->data, renderer);
    }
}

void PlotLayer_update(PlotLayer* self){
    if(self != NULL && 
       self->data != NULL){

        int current_layer_size = ((Object*)self)->width;

        int NbPointsToDisplay = ( (current_layer_size + 1) / self->data->resolution );

        plot_set_displayable_points_count(self->data, NbPointsToDisplay);

        plot_update(self->data);

        // rescale
        // Here we set the transformation from Subplot Frame to Data Pixel Frame ( Layer Frame )
        // We convert all real data into pixels represented in subplot/Layer frame
        Frame_t f;

        f.parent = &self->base.frame;

        /* Map the visible view window onto the layer's pixel rectangle. The
           y term is negated so data-y increases upward on screen. */
        f.Hom.R.rxx = (double)self->base.width  / (self->data->x_view_max - self->data->x_view_min);
        f.Hom.R.ryx = 0;
        f.Hom.R.rxy = 0;
        f.Hom.R.ryy = (double)self->base.height / (self->data->y_view_min - self->data->y_view_max);

        f.Hom.t.u = -f.Hom.R.rxx * self->data->x_view_min;
        f.Hom.t.v = -f.Hom.R.ryy * self->data->y_view_max;
        f.Hom.t.s = 1;
        
        Object_SetFrame((Object*)(&(*self->data)), &f);
    }
}

void PlotLayer_Destroy(void* layer){
    if(layer != NULL){
        if(((PlotLayer*)layer)->data != NULL){
            free(((PlotLayer*)layer)->data);
        }
        free(layer);
    }
}

int PlotLayer_SetData(PlotLayer* self, plot* data){
    if(self != NULL
        && data != NULL){
        self->data = data;
        return 0;
    }
    return -1;
}

void PlotLayer_render(void* self, void* renderer){
    if(self != NULL){
        PlotLayer_update((PlotLayer*)self);
        if(renderer != NULL){
            PlotLayer_plot_data((PlotLayer*)self,(SDL_Renderer*)renderer);
        }
    }
}

PlotLayer* PlotLayer_Create(){
    PlotLayer* layer = (PlotLayer*)malloc(sizeof(PlotLayer));
    if(layer == NULL){
        return NULL;
    }
    layer->data = NULL;
    layer->base.render = PlotLayer_render;
    layer->base.destroy = PlotLayer_Destroy;
    return layer;
}