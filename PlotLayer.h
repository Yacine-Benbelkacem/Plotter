#ifndef PlOTLAYER_H
#define PLOTLAYER_H

#include "common_types.h"

#define PLOTLAYER_RESOLUTION 1 //pixels per point

typedef plot plot;


typedef struct PlotLayer{
    plot* data; //plot
    PlottingArea* area; //plotting area

    int pxl_resolution;

    float x_resampledValues[10000];
    float y_resampledValues[10000];
    int y_pxlValues[10000];
    int NbPointsToDisplay;

    int max_val_idx;
    int min_val_idx;

}PlotLayer;


void resample(PlotLayer * layer);

int rescale(PlotLayer * layer);

void PlotLayer_plot(PlotLayer* layer);
void PlotLayer_plot_update(PlotLayer* layer);
PlotLayer* PlotLayer_Create(plot* p, PlottingArea* area);


#endif // PLOTLAYER_H