#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <Frame.h>
#include <Object.h>
#include <Figure.h>
#include <Plot.h>


int main(){
    double rot[4] = {1,  0, 
                     0, -1}; // To SDL window origin
    
    printf("Test Plot.c module \n");

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

    for(int i = 0; i<100; i++){
        double x = 2*M_PI*i*0.01;
        plot_insert_point(p, x, sin(x));
        printf("x = %f, f(x) = %f\n", p->points[i].x,p->points[i].y);
    }

    plot_set_nb_points_to_display(p,20);

    plot_update(p);

    
    printf("%d \n", p->num_points_displayed);
    printf("%f \n", p->y_min_displayed_point);
    printf("%f \n", p->y_max_displayed_point);
    
    for(int i = 0; i < p->num_points_displayed; i++){
        printf("x = %f, f(x) = %f\n", p->displayed_points[i].x,p->displayed_points[i].y);
    }   



    return 0;
}