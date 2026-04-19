#include "Figure.h"
#include "PlottingArea.h"
#include "PlotLayer.h"


Figure* Figure_Create(){
    Figure* figure = (Figure*)malloc(sizeof(Figure));
    figure->window = SDL_CreateWindow(
                        "Plotter",
                        SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED,
                        FIGURE_MIN_WIDTH*2,
                        FIGURE_MIN_HEIGHT*2,
                        SDL_WINDOW_RESIZABLE
                    );
    SDL_SetWindowMinimumSize(figure->window, FIGURE_MIN_WIDTH, FIGURE_MIN_HEIGHT);
    
    figure->renderer = SDL_CreateRenderer(figure->window, -1, SDL_RENDERER_ACCELERATED); 

    figure->plotting_area = PlottingArea_Create(figure);

    return figure;
}

void Figure_Update(Figure* figure){
    
    SDL_SetRenderDrawColor(figure->renderer, 0, 0, 0, 255);
    SDL_RenderClear(figure->renderer);
    PlottingArea_Update(figure->plotting_area);
    SDL_RenderPresent(figure->renderer);    
    
}


void Figure_plot(Figure* figure, plot* p){
    if(figure != NULL && figure->plotting_area != NULL){
        PlottingArea* area = figure->plotting_area;
        PlotLayer_Create(p, area);
    }
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