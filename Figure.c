#include <math.h>
#include <string.h>

#include "Figure.h"
#include "Frame.h"
#include "Subplot.h"
#include "Text.h"

#define DEFAULT_FONT    "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
#define DEFAULT_FONT_PT 13
#define ZOOM_STEP       0.85   /* view shrink factor per wheel notch */


Figure* Figure_create(const char* title){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return NULL;
    }
    if(Text_Init(DEFAULT_FONT, DEFAULT_FONT_PT) != 0){
        SDL_Log("Warning: could not load font '%s'; text will not render.", DEFAULT_FONT);
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

int32_t Figure_plot(Figure* self, plot* plt){
    int32_t idx = Figure_add_subplot(self);
    if(idx < 0) return -1;
    Subplot_AddPlot((Subplot*)self->subplots[idx], plt);
    return idx;
}

/* Route a wheel event at (mx,my) to whichever subplot contains the cursor. */
static void Figure_handle_zoom(Figure* figure, int mx, int my, int wheel_y){
    if(wheel_y == 0) return;
    double factor = (wheel_y > 0) ? ZOOM_STEP : 1.0 / ZOOM_STEP;
    for(int i = 0; i < figure->subplot_count; i++){
        if(figure->subplots[i] != NULL){
            if(Subplot_ZoomAt((Subplot*)figure->subplots[i], mx, my, factor)){
                break;
            }
        }
    }
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
            switch(event.type){
                case SDL_QUIT:
                    running = SDL_FALSE;
                    break;
                case SDL_MOUSEWHEEL: {
                    int mx, my;
                    SDL_GetMouseState(&mx, &my);
                    Figure_handle_zoom(figure, mx, my, event.wheel.y);
                    break;
                }
                default:
                    break;
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

    Text_Quit();
    SDL_DestroyRenderer(figure->renderer);
    SDL_DestroyWindow(figure->window);
    SDL_Quit();
    free(figure);
}
