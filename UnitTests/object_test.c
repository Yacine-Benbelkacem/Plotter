#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <Frame.h>
#include <Object.h>


int main(){
    double rot[4] = {1,  0, 
                     0, -1}; // To SDL window origin
    
    printf("Test Frame.c module \n");

    Vect pos = {
        .u = 0,
        .v = 20, //cm
        .s = 1, // scaled to 1
    };

    Vect dims = {
        .u = 20,
        .v = 20, //cm
        .s = 1, // scaled to 1
    };

    Frame_t* f0 = NULL;
    Frame_pInit(&f0);
    Frame_SetOrigin(f0, pos.u, pos.v);
    Frame_SetRotation(f0, rot);
    Frame_SetScale(f0, dims.s);
    
    printFrame(f0);

    int32_t w = 25;
    int32_t h = 25;
    Object* obj = Object_Init(&w, &h, f0);

    print_object(obj);

    Frame_Destroy(f0);
    Object_Destroy(obj);

    return 0;
}