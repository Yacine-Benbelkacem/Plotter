#ifndef CURVELAYER_H
#define CURVELAYER_H

#include "common_types.h"
#include "Object.h"
#include "Plot.h"

#define CURVELAYER_RESOLUTION 1 //pixels per point



typedef struct CurveLayer{
    Object base; // Inherit from Object
    plot* data; //plot
    point pxlValues[10000];
}CurveLayer;

CurveLayer* CurveLayer_Create();
void CurveLayer_plot(CurveLayer* layer);
int CurveLayer_SetData(CurveLayer* self, plot* data);
void CurveLayer_update(CurveLayer* self);
void CurveLayer_Destroy(void* layer);


#endif // CURVELAYER_H