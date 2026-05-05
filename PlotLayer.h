#ifndef PlOTLAYER_H
#define PLOTLAYER_H

#include "common_types.h"
#include "Plot.h"

#define PLOTLAYER_RESOLUTION 1 //pixels per point



typedef struct PlotLayer{
    Object base; // Inherit from Object
    plot* data; //plot
    point pxlValues[10000];
}PlotLayer;

PlotLayer* PlotLayer_Create();
void resample(PlotLayer * layer, int num_points);
void PlotLayer_plot(PlotLayer* layer);
int PlotLayer_SetData(PlotLayer* self, plot* data);
void PlotLayer_update(PlotLayer* self);
void PlotLayer_Destroy(void* layer);


#endif // PLOTLAYER_H