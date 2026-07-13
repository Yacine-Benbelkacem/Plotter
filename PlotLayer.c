#include "Figure.h"
#include "Plot.h"
#include "PlotLayer.h"
#include "Frame.h"
#include <math.h>


void PlotLayer_show_plotZone(PlotLayer* layer,SDL_Renderer* renderer){
    if(layer != NULL){
        // draw lines
        SDL_SetRenderDrawColor(renderer,0,0,0,155);
        SDL_Rect PlotZone = {
            .w = layer->base.width+4,
            .h = layer->base.height+4,
            .x = layer->base.frame.Hom.t.u-2,
            .y = layer->base.frame.Hom.t.v-2,
        };
        SDL_RenderDrawRect(renderer,&PlotZone);
    }
}

void PlotLayer_plot_data(PlotLayer* layer,SDL_Renderer* renderer){
    if(layer != NULL && 
       layer->data != NULL){
        // draw lines
        PlotLayer_show_plotZone(layer, renderer);
        
        ((Object*)layer->data)->render((Object*)layer->data, renderer);

    }
}

void PlotLayer_update(PlotLayer* self){
    if(self != NULL && 
       self->data != NULL){

        int num_data_points = self->data->current_points_count;
        int current_layer_size = ((Object*)self)->width;

        int NbPointsToDisplay = ( (current_layer_size + 1) / self->data->resolution );

        plot_set_displayable_points_count(self->data, NbPointsToDisplay);

        plot_update(self->data);

        // rescale
        // Here we set the transformation from Subplot Frame to Data Pixel Frame ( Layer Frame )
        // We convert all real data into pixels represented in subplot/Layer frame
        Frame_t f;

        f.parent = &self->base.frame;

        f.Hom.R.rxx = (double)self->base.width  / (self->data->x_max_displayed_point - self->data->x_min_displayed_point);
        f.Hom.R.ryx = 0;
        f.Hom.R.rxy = 0;
        f.Hom.R.ryy = (double)self->base.height / (self->data->y_min_displayed_point - self->data->y_max_displayed_point);

        f.Hom.t.u = -f.Hom.R.rxx * self->data->x_min_displayed_point;
        f.Hom.t.v = -f.Hom.R.ryy * self->data->y_max_displayed_point;
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