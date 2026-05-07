#include <math.h>
#include <unistd.h>

#include "Object.h"
#include "Figure.h"
#include "Subplot.h"
#include "Plot.h"

Figure* Figure_create(const char* title){
    Figure* figure = (Figure*)malloc(sizeof(Figure));
    for(int i = 0; i < 128; i++){
        figure->subplots[i] = NULL;
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

    figure->subplot_count = 0;
 
    return figure;
}

void Figure_get_size(const Figure * figure, int32_t * width, int32_t height) {
    SDL_GetWindowSize(figure->window,
                      &width, 
                      &height);
}


int32_t Figure_add_subplot(Figure* figure)
{
    if (figure == NULL) {
        return -1;
    }
    
    Subplot* subplot = Subplot_Create();

    if ((subplot != NULL)
         && (figure->subplot_count < MAX_SUBPLOTS)) {

        figure->subplots[figure->subplot_count++] = 
            (Object*)subplot;
        
        return ((Object*)subplot)->id;
    }

    return -1;
}

void Figure_plot(Figure* self, plot* plt){
    int sub_id = Figure_add_subplot(self);
    Subplot* sub = (Subplot*)self->subplots[sub_id];
    Subplot_AddPlot(sub,plt);
}


void Figure_update_layout(Figure* figure)
{
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

    int width, height, width_per_subplot, height_per_subplot;
    Frame_t subplot_frame;

    Frame_SetRotation(&subplot_frame,
                        1.0, 0.0,
                        0.0, 1.0);

    Figure_get_size(figure, &width, &height);

    width_per_subplot = 
        (figure->subplot_count > 1) ? width / 2 : width;  // Max of 2 columns in the figure 

    height_per_subplot = 
        height / (ceil(figure->subplot_count / 2.0));

    for (int i = 0; i < figure->subplot_count; i++) 
    {
        if (figure->subplots[i] != NULL)
        {
            Frame_SetOrigin(&subplot_frame, 
                            (i%2 == 0 ? 0 : width_per_subplot),
                            (i/2)*height_per_subplot);

            Object_SetSize((Object*)figure->subplots[i],
                            width_per_subplot, 
                            height_per_subplot);

            Object_SetFrame((Object*)figure->subplots[i], 
                             &subplot_frame);
        }
    }
}

void Figure_render(Figure* figure) {

    // background color : black
    SDL_SetRenderDrawColor(figure->renderer, 0, 0, 0, 255);
    SDL_RenderClear(figure->renderer);

    for(int i = 0; i < figure->subplot_count; i++){
        if(figure->subplots[i] != NULL){
            if(figure->subplots[i]->render != NULL){
                figure->subplots[i]->render(figure->subplots[i], figure->renderer);
            }
        }
    }

    SDL_RenderPresent(figure->renderer);
}

void Figure_update(Figure* figure)
{
    Figure_update_layout(figure);
    Figure_render(figure);
}

void Figure_show(Figure* figure) {
    if(figure != NULL) 
    {
        SDL_bool running = SDL_TRUE;
        SDL_Event event;

        while (running) 
        {
            while (SDL_PollEvent(&event)) 
            {
                if (event.type == SDL_QUIT) {
                    running = SDL_FALSE;
                }
            }

            Figure_update(figure);

            sleep(0.033);
        }

        SDL_DestroyRenderer(figure->renderer);
        SDL_DestroyWindow(figure->window);
        SDL_Quit();
    }
}
