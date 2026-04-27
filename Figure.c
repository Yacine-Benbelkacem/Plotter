#include "Object.h"
#include "Figure.h"


static int object_counter = 0;

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

    return figure;
}

void Figure_Update(Figure* figure){
    
    SDL_SetRenderDrawColor(figure->renderer, 0, 0, 0, 255);
    SDL_RenderClear(figure->renderer);
    
    for(int i = 0; i < MAX_OBJECTS; i++){
        if(figure->objects[i] != NULL && figure->objects[i]->render != NULL){
            figure->objects[i]->render(figure->objects[i], figure->renderer);
        }
    }

    SDL_RenderPresent(figure->renderer);

}

int Figure_add_object(Figure* figure, Object* obj){
    if(figure != NULL && obj != NULL && object_counter < 128){
        figure->objects[object_counter++] = obj;
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
            
        }

        SDL_DestroyRenderer(figure->renderer);
        SDL_DestroyWindow(figure->window);
        SDL_Quit();
    }
}