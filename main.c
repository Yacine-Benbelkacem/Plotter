#include <SDL2/SDL.h>
#include <math.h>
#include "Plotter.h"

#define PI 3.14159265
#define frq 10 //Hz
int main(){

    plot* p = plot_init(10000);

    for(int i = 0; i < 10000; i++){
        float x = i * 0.0001f; // time in seconds
        float y = sin(2*PI*frq * x); // sine wave value at time x
        plot_add_point(p, x, y);
    }

    Figure* figure = Figure_Create("POC");

    Figure_plot(figure, p);

    Figure_Show(figure);

    return 0;
}