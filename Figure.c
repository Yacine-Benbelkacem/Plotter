#include "Object.h"
#include "Figure.h"
#include "Subplot.h"
#include <math.h>
#include <unistd.h>

Figure* Figure_Create(const char* title){
    Figure* figure = (Figure*)malloc(sizeof(Figure));
    for(int i = 0; i < 128; i++){
        figure->objects[i] = NULL;
    }
    figure->window = SDL_CreateWindow(
                        title,
                        SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED,
                        FIGURE_MIN_WIDTH*2,
                        FIGURE_MIN_HEIGHT*2,
                        SDL_WINDOW_RESIZABLE
                    );
    SDL_SetWindowMinimumSize(figure->window, FIGURE_MIN_WIDTH, FIGURE_MIN_HEIGHT);

    figure->renderer = SDL_CreateRenderer(figure->window, -1, SDL_RENDERER_ACCELERATED); 

    figure->object_counter = 0;
 
    return figure;
}

void Figure_Update(Figure* figure){
    
    SDL_SetRenderDrawColor(figure->renderer, 0, 0, 0, 255);
    SDL_RenderClear(figure->renderer);
    //SDL_RenderFillRect(figure->renderer, NULL);
    
    
    Figure_update_layout(figure);

    for(int i = 0; i < figure->object_counter; i++){
        if(figure->objects[i] != NULL){
            if(figure->objects[i]->render != NULL){
                figure->objects[i]->render(figure->objects[i], figure->renderer);
            }
        }
    }

    SDL_RenderPresent(figure->renderer);

}

int Figure_add_object(Figure* figure, Object* obj){
    if(figure != NULL && obj != NULL && figure->object_counter < 128){
        figure->objects[figure->object_counter] = (Object*)obj;
        figure->object_counter++;
        return 0;
    }
    return -1;
}

void Figure_Show(Figure* figure){
    if(figure != NULL){
        SDL_bool running = SDL_TRUE;
        SDL_Event event;
        while (running) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = SDL_FALSE;
                }
            }

            Figure_Update(figure);

            sleep(0.033);
            
        }

        SDL_DestroyRenderer(figure->renderer);
        SDL_DestroyWindow(figure->window);
        SDL_Quit();
    }
}

int Figure_add_subplot(Figure* figure){
    Subplot* subplot = Subplot_Create();
    if(subplot != NULL){

        Figure_add_object(figure, (Object*)subplot);
        
        return ((Object*)subplot)->id;
    }
    return 0;
}

void Figure_update_layout(Figure* figure){ 
    int width, height;
    SDL_GetWindowSize(figure->window, &width, &height);
    int mean_width = (figure->object_counter>1) ? width / 2 : width; // Assuming 2 columns for simplicity   
    int mean_height = height / (ceil(figure->object_counter / 2.0));


    for(int i = 0; i < figure->object_counter; i++){
        if(figure->objects[i] != NULL){
            Object_SetSize((Object*)figure->objects[i], mean_width, mean_height);
            Frame_t frame = {
                .parent = NULL,
                .Hom.R = { .rxx = 1.0, .rxy = 0.0, 
                           .ryx = 0.0, .ryy = 1.0 },
                .Hom.t = { .u = (i%2 == 0 ? 0 : mean_width), 
                           .v = (i/2)*mean_height, 
                           .s = 1.0 }
            };
            Object_SetFrame((Object*)figure->objects[i], &frame);
        }
    }
}