#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <Frame.h>
#include <Object.h>
#include <Figure.h>


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

    Figure* fig = Figure_Create("Test Subplot");
    int sub_id = Figure_add_subplot(fig);
    printf("Added subplot with ID: %d\n", sub_id);
    int sub_id = Figure_add_subplot(fig);
    printf("Added subplot with ID: %d\n", sub_id);
    
    Figure_Show(fig);

    return 0;
}