#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <Frame.h>
#include <Object.h>
#include <Figure.h>
#include <PlotLayer.h>
#include <Plot.h>

int main(){
    double rot[4] = {1,  0, 
                     0, -1}; // To SDL window origin
    
    printf("Test Subplot.c module \n");

    Vect pos = {
        .u = 20,
        .v = 20, //cm
        .s = 1, // scaled to 1
    };

    Vect dims = {
        .u = 20,
        .v = 20, //cm
        .s = 1, // scaled to 1
    };

    int32_t w = 25;
    int32_t h = 25;

    plot * p = plot_init(10000);

    for(int i = 0; i<1000; i++){
        double x = 2*M_PI*i*0.01;
        plot_add_point(p, x, sin(x));
    }

   

    plot_update(p);


    PlotLayer * pltlayer = PlotLayer_Create();
    pltlayer->base.height = 500;
    pltlayer->base.width = 500;
    
    PlotLayer_SetData(pltlayer,p);
    PlotLayer_update(pltlayer);
    
    PlotLayer_Destroy(pltlayer);

    return 0;
}