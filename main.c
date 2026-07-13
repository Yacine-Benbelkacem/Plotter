#include <SDL2/SDL.h>
#include <math.h>
#include "Plotter.h"

#define PI 3.14159265
#define frq 10 //Hz
int main(){

    plot* p = plot_init(10000);

    for(int i = 0; i < 10000; i++){
        double x = i * 0.0001f; // time in seconds
        double y = sin(2*PI*frq * x); // sine wave value at time x
        plot_insert_point(p, x, y);
    }

    Figure* figure = Figure_create("POC");

    Figure_plot(figure, p);

    Figure_show(figure);

    return 0;
}