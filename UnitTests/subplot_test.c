#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <Frame.h>
#include <Object.h>
#include <Figure.h>
#include <Subplot.h>
#include <Plot.h>


int main(){

    printf("Test Subplot.c module \n");


    plot * p = plot_init(10000);

    for(int i = 0; i<1000; i++){
        double x = 2*M_PI*i*0.01;
        plot_insert_point(p, x, sin(x));
    }

    Figure* fig = Figure_Create("Test Subplot");
    int sub_id = Figure_add_subplot(fig);

    Subplot* sub1 = (Subplot*)fig->objects[sub_id];
    Subplot_AddPlot(sub1,p);
    sub_id = Figure_add_subplot(fig);
    sub_id = Figure_add_subplot(fig);
    sub_id = Figure_add_subplot(fig);

    Figure_Show(fig);

    return 0;
}