#include <math.h>
#include <string.h>

#include "Figure.h"
#include "Frame.h"
#include "Subplot.h"


Figure* Figure_create(const char* title){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return NULL;
    }
    Figure* figure = malloc(sizeof(Figure));
    if(figure == NULL){
        return NULL;
    }
    memset(figure->subplots, 0, sizeof(figure->subplots));

    figure->window = SDL_CreateWindow(
                        title,
                        SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED,
                        FIGURE_MIN_WIDTH * 2,
                        FIGURE_MIN_HEIGHT * 2,
                        SDL_WINDOW_RESIZABLE
                    );
    SDL_SetWindowMinimumSize(figure->window, FIGURE_MIN_WIDTH, FIGURE_MIN_HEIGHT);

    figure->renderer = SDL_CreateRenderer(figure->window, -1, SDL_RENDERER_ACCELERATED);

    figure->subplot_count = 0;

    return figure;
}

void Figure_get_size(const Figure* figure, int32_t* width, int32_t* height){
    SDL_GetWindowSize(figure->window, width, height);
}

int32_t Figure_add_subplot(Figure* figure){
    if(figure == NULL || figure->subplot_count >= MAX_SUBPLOTS){
        return -1;
    }

    Subplot* subplot = Subplot_Create();
    if(subplot == NULL){
        return -1;
    }

    int32_t idx = figure->subplot_count;
    figure->subplots[figure->subplot_count++] = (Object*)subplot;
    return idx;
}

void Figure_plot(Figure* self, plot* plt){
    int32_t idx = Figure_add_subplot(self);
    if(idx < 0) return;
    Subplot_AddPlot((Subplot*)self->subplots[idx], plt);
}

void Figure_update_layout(Figure* figure){
    /* Figure layout */
    /*
    .--------------------------------------------------------.
    |                      Figure Title                       |
    |o--->-----------------------o--->------------------------|
    || frame11 == figure frame   | frame12                    |
    |v                           v                            |
    |        Subplot11           |        Subplot12           |
    |                            |                            |
    |                            |                            |
    |o--->-----------------------o--->------------------------|
    || frame21                   | frame22                    |
    |v                           v                            |
    |        Subplot21           |        Subplot22           |
    |                            |                            |
    |____________________________|____________________________|
    */

    int32_t width, height, width_per_subplot, height_per_subplot;
    Frame_t subplot_frame;
    subplot_frame.parent = NULL;

    Frame_SetRotation(&subplot_frame, 1.0, 0.0, 0.0, 1.0);

    Figure_get_size(figure, &width, &height);

    width_per_subplot =
        (figure->subplot_count > 1) ? width / 2 : width;

    height_per_subplot =
        height / (int32_t)ceil(figure->subplot_count / 2.0);

    for(int i = 0; i < figure->subplot_count; i++){
        if(figure->subplots[i] != NULL){
            Frame_SetOrigin(&subplot_frame,
                            (i % 2 == 0) ? 0 : width_per_subplot,
                            (i / 2) * height_per_subplot);

            Object_SetSize((Object*)figure->subplots[i],
                            width_per_subplot,
                            height_per_subplot);

            Object_SetFrame((Object*)figure->subplots[i], &subplot_frame);
        }
    }
}

static void Figure_render(Figure* figure){
    SDL_SetRenderDrawColor(figure->renderer, 0, 0, 0, 255);
    SDL_RenderClear(figure->renderer);

    for(int i = 0; i < figure->subplot_count; i++){
        if(figure->subplots[i] != NULL && figure->subplots[i]->render != NULL){
            figure->subplots[i]->render(figure->subplots[i], figure->renderer);
        }
    }

    SDL_RenderPresent(figure->renderer);
}

void Figure_update(Figure* figure){
    Figure_update_layout(figure);
    Figure_render(figure);
}

void Figure_show(Figure* figure){
    if(figure == NULL) return;

    SDL_bool running = SDL_TRUE;
    SDL_Event event;

    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                running = SDL_FALSE;
            }
        }
        Figure_update(figure);
        SDL_Delay(33);
    }

    for(int i = 0; i < figure->subplot_count; i++){
        if(figure->subplots[i] != NULL && figure->subplots[i]->destroy != NULL){
            figure->subplots[i]->destroy(figure->subplots[i]);
        }
    }

    SDL_DestroyRenderer(figure->renderer);
    SDL_DestroyWindow(figure->window);
    SDL_Quit();
    free(figure);
}
