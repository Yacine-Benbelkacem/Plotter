#include "Figure.h"
#include "Plot.h"
#include "CurveLayer.h"
#include "Frame.h"
#include <math.h>


void CurveLayer_show_plotZone(CurveLayer* layer,SDL_Renderer* renderer){
    if(layer != NULL){
        // draw lines
        SDL_SetRenderDrawColor(renderer,0,0,0,155);
        SDL_Rect PlotZone = {
            .w = layer->base.width+4,
            .h = layer->base.height+4,
            .x = layer->base.frame.Hom.t.u-2,
            .y = layer->base.frame.Hom.t.v - layer->base.height/2 - 2
        };
        SDL_RenderDrawRect(renderer,&PlotZone);
    }
}

void CurveLayer_plot_data(CurveLayer* layer,SDL_Renderer* renderer){
    if(layer != NULL && 
       layer->data != NULL){
        // draw lines
        CurveLayer_show_plotZone(layer, renderer);
        SDL_SetRenderDrawColor(renderer,255,0,0,255);
        for(int i=0; i<layer->data->displayed_points_count-1; i++){
            SDL_RenderDrawLine(renderer, 
                               layer->pxlValues[i].x,
                               layer->pxlValues[i].y,
                               layer->pxlValues[i+1].x,
                               layer->pxlValues[i+1].y);
        }
    }
}

void CurveLayer_Data2Pixel(CurveLayer* layer){
    for(int i = 0; i < layer->data->displayed_points_count; i++){
        Vect res = Frame_multHV(
                    layer->base.frame.Hom, 
                    (Vect){.u = layer->data->displayed_points[i].x,
                           .v = layer->data->displayed_points[i].y,
                           .s = 1});
        layer->pxlValues[i].x = res.u;
        layer->pxlValues[i].y = res.v;
    }
}

void CurveLayer_update(CurveLayer* self){
    if(self != NULL && 
       self->data != NULL){

        int num_data_points = self->data->current_point_idx;
        int current_layer_size = ((Object*)self)->width;

        int NbPointsToDisplay = ( (current_layer_size + 1) / self->data->resolution );

        plot_set_nb_points_to_display(self->data, NbPointsToDisplay);

        plot_update(self->data);

        // rescale
        // Here we set the transformation from Subplot Frame to Data Pixel Frame ( Layer Frame )
        // We convert all real data into pixels represented in subplot/Layer frame
        Frame_t * f = &self->base.frame;

        f->Hom.R.rxx = (float)self->base.width/(self->data->x_max_displayed_point-self->data->x_min_displayed_point);
        f->Hom.R.ryx = 0;
        f->Hom.R.rxy = 0; 
        f->Hom.R.ryy = (float)self->base.height/(self->data->y_min_displayed_point-self->data->y_max_displayed_point);

        f->Hom.t.u = f->Hom.t.u + (-f->Hom.R.rxx * self->data->x_min_displayed_point);
        f->Hom.t.v = f->Hom.t.v + (-f->Hom.R.ryy * self->data->y_max_displayed_point);
        f->Hom.t.s = 1;
        
        CurveLayer_Data2Pixel(self);
    }
}

void CurveLayer_Destroy(void* layer){
    if(layer != NULL){
        if(((CurveLayer*)layer)->data != NULL){
            free(((CurveLayer*)layer)->data);
        }
        free(layer);
    }
}

int CurveLayer_SetData(CurveLayer* self, plot* data){
    if(self != NULL
        && data != NULL){
        self->data = data;
        return 0;
    }
    return -1;
}

void CurveLayer_render(void* self, void* renderer){
    if(self != NULL){
        CurveLayer_update((CurveLayer*)self);
        if(renderer != NULL){
            CurveLayer_plot_data((CurveLayer*)self,(SDL_Renderer*)renderer);
        }
    }
}

CurveLayer* CurveLayer_Create(){
    CurveLayer* layer = (CurveLayer*)malloc(sizeof(CurveLayer));
    if(layer == NULL){
        return NULL;
    }
    layer->data = NULL;
    layer->base.render = CurveLayer_render;
    layer->base.destroy = CurveLayer_Destroy;
    return layer;
}