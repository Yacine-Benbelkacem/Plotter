#include "Figure.h"
#include "PlottingArea.h"
#include "PlotLayer.h"
#include "Plot.h"
#include "Frame.h"
#include <math.h>

void PlotLayer_render(void* self, void* renderer){
    PlotLayer* layer = (PlotLayer*)self;
    if(layer != NULL){
        PlotLayer_plot_update(layer);
    }
}


void PlotLayer_plot_data(PlotLayer* layer){
    if(layer != NULL && 
       layer->data != NULL){
        // draw lines
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

        int num_data_points = self->data->num_points;
        int current_layer_size = ((Object*)self)->width;

        int NbPointsToDisplay = ( (current_layer_size + 1) / PLOTLAYER_RESOLUTION );

        printf("NbPointsToDisplay: %d\n", NbPointsToDisplay);

        plot_set_nb_points_to_display(self->data, NbPointsToDisplay);

        plot_update(self->data);

        // rescale
        Frame_t * f = &self->base.frame;
        
        f->Hom.R.rxx = 1;
        f->Hom.R.ryx = 0;
        f->Hom.R.rxy = 0; 
        f->Hom.R.ryy = (double)self->base.height/(self->data->y_min_displayed_point-self->data->y_max_displayed_point);

        f->Hom.t.u = 0;
        f->Hom.t.v = f->Hom.R.ryy * self->base.height;
        f->Hom.t.s = 1;

        PlotLayer_Data2Pixel(self);
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
    layer->base.render = PlotLayer_render;
    layer->base.destroy = PlotLayer_Destroy;
    return layer;
}

int PlotLayer_SetData(PlotLayer* self, plot* data){
    if(self != NULL
        && data != NULL){
        self->data = data;
        return 0;
    }
    return -1;
}