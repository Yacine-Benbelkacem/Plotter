#include <stdio.h>
#include <SDL2/SDL.h>
#include "../Frame.h"


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

    Frame f0 = {
        .parent = NULL,
    };
    
    SDL_Window* window = SDL_CreateWindow(
                        "Plotter",
                        SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED,
                        800,
                        800,
                        SDL_WINDOW_RESIZABLE
                    );
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    SDL_Rect rect ;

    SDL_bool running = SDL_TRUE;
    SDL_Event event;

    int H = 0;
    int W = 0;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = SDL_FALSE;
            }
        }

        SDL_GetWindowSize(window, &W, &H);
        f0.Hom.t.u = 0;
        f0.Hom.t.v = H;

        
        Frame_SetScale(&f0, (H < W ? H : W) / 100.0); // width and height are in cm, so we scale them to fit the window size
                                                      // 100 represents 1m in cm, so we scale the dimensions to fit the window size

        Frame_SetRotation(&f0, rot);

        Vect pos_0 = Frame_multHV(f0.Hom, pos);
        Vect dims_0 = Frame_multHV(f0.Hom, dims);

        rect.x = pos_0.u;
        rect.y = pos_0.v;
        rect.w = dims_0.u;
        rect.h = dims_0.v;
        
        printf("dims %f dims_0 %f\n",dims.u, dims_0.u );

        printf("repos_0.u = %f, pos_0.v %f\n", pos_0.u, pos_0.v);
        
        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);

        SDL_RenderDrawRect(renderer, &rect);
        
        SDL_RenderPresent(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}