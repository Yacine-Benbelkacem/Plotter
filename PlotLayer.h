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


void resample(PlotLayer * layer, int num_points);

int rescale(PlotLayer * layer);

void PlotLayer_plot(PlotLayer* layer);
int PlotLayer_SetData(PlotLayer* self, plot* data);
void PlotLayer_plot_update(PlotLayer* layer);
PlotLayer* PlotLayer_Create();


#endif // PLOTLAYER_H