#include "Plot.h"
#include <stdlib.h>

plot* plot_init(int data_legnth){
    plot* p = (plot*)malloc(sizeof(plot));
    p->points = (point*)malloc(data_legnth * sizeof(point));
    p->data_legnth = data_legnth;
    return p;
}