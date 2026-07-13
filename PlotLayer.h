#ifndef PLOTLAYER_H
#define PLOTLAYER_H

#include "common_types.h"
#include "Object.h"
#include "Plot.h"

#define PlotLayer_RESOLUTION 1

typedef struct PlotLayer{
    Object base;
    plot*  data;
}PlotLayer;

PlotLayer* PlotLayer_Create();
int        PlotLayer_SetData(PlotLayer* self, plot* data);
void       PlotLayer_update(PlotLayer* self);
void       PlotLayer_Destroy(void* layer);

#endif // PLOTLAYER_H
