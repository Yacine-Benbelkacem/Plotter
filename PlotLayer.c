#include "Figure.h"
#include "Plot.h"
#include "PlotLayer.h"
#include "Frame.h"
#include <math.h>

void PlotLayer_plot_data(PlotLayer* layer,SDL_Renderer* renderer){
    if(layer != NULL && 
       layer->data != NULL){
        // draw lines
        SDL_SetRenderDrawColor(renderer,255,255,0,255);
        for(int i=0; i<layer->data->num_points_displayed-1; i++){
            SDL_RenderDrawLine(renderer, 
                               layer->pxlValues[i].x,
                               layer->pxlValues[i].y,
                               layer->pxlValues[i+1].x,
                               layer->pxlValues[i+1].y);
        }
    }
}

void PlotLayer_Data2Pixel(PlotLayer* layer){
    for(int i = 0; i < layer->data->num_points_displayed; i++){
        Vect res = Frame_multHV(
                    layer->base.frame.Hom, 
                    (Vect){.u = layer->data->displayed_points[i].x,
                           .v = layer->data->displayed_points[i].y,
                           .s = 1});
        layer->pxlValues[i].x = res.u;
        layer->pxlValues[i].y = res.v;
    }
}

void PlotLayer_update(PlotLayer* self){
    if(self != NULL && 
       self->data != NULL){

        int num_data_points = self->data->current_point_idx;
        int current_layer_size = ((Object*)self)->width;

        int NbPointsToDisplay = ( (current_layer_size + 1) / PLOTLAYER_RESOLUTION );

        printf("NbPointsToDisplay: %d\n", NbPointsToDisplay);

        plot_set_nb_points_to_display(self->data, NbPointsToDisplay);

        plot_update(self->data);

        // rescale
        Frame_t * f = &self->base.frame;

        f->Hom.R.rxx = (float)self->base.width/(self->data->x_max_displayed_point-self->data->x_min_displayed_point);
        printf("rxx %f \n", f->Hom.R.rxx );
        f->Hom.R.ryx = 0;
        f->Hom.R.rxy = 0; 
        f->Hom.R.ryy = (float)self->base.height/(self->data->y_min_displayed_point-self->data->y_max_displayed_point);
        printf("ryy %f \n", f->Hom.R.ryy);

        f->Hom.t.u = (-f->Hom.R.rxx * self->data->x_min_displayed_point);
        f->Hom.t.v = (-f->Hom.R.ryy * self->data->y_max_displayed_point);
        f->Hom.t.s = 1;
        printf("tx %f \n", f->Hom.t.u );
        printf("ty %f \n", f->Hom.t.v );
        
        PlotLayer_Data2Pixel(self);
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