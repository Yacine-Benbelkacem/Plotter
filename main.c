#include <SDL2/SDL.h>
#include <math.h>
#include "Plotter.h"

#define PI 3.14159265
#define frq 10 //Hz
int main(){

    plot* p = plot_init(10000);

    for(int i = 0; i < 10000; i++){
        p->points[i].x = i * 0.0001f;
        p->points[i].y = sin(2*PI*frq * p->points[i].x);
    }

    Figure* figure = Figure_Create();

    Figure_plot(figure, p);

    Figure_Show(figure);

    return 0;
}