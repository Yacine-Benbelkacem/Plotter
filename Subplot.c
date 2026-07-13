#include "Subplot.h"
#include "Frame.h"
#include "PlotLayer.h"

#define MAX_SUBPLOT_LAYERS 10

static int subplot_idx_count = 0;

void Subplot_Render(void* self, void* renderer){
    Subplot* subplot = (Subplot*)self;
    SDL_Renderer* rend = (SDL_Renderer*)renderer;

    Subplot_Update(subplot);

    SDL_RenderSetViewport(rend, subplot->viewport);
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    SDL_RenderFillRect(rend, NULL);

    subplot->base.frame.Hom.t.u = 0;
    subplot->base.frame.Hom.t.v = 0;

    for(int i = 0; i < subplot->subplot_layers_count; i++){
        if(subplot->layers[i] != NULL){
            ((Object*)subplot->layers[i])->render(subplot->layers[i], renderer);
        }
    }
}

void Subplot_Destroy(void* subplot_ptr){
    Subplot* subplot = (Subplot*)subplot_ptr;
    if(subplot == NULL) return;

    if(subplot->viewport != NULL){
        free(subplot->viewport);
    }
    for(int i = 0; i < subplot->subplot_layers_count; i++){
        if(subplot->layers[i] != NULL){
            Object* obj = (Object*)subplot->layers[i];
            if(obj->destroy != NULL){
                obj->destroy(subplot->layers[i]);
            } else {
                free(subplot->layers[i]);
            }
        }
    }
    free(subplot);
}

Subplot* Subplot_Create(){
    Subplot* subplot = malloc(sizeof(Subplot));
    if(subplot == NULL) return NULL;

    subplot->viewport = malloc(sizeof(SDL_Rect));
    if(subplot->viewport == NULL){
        free(subplot);
        return NULL;
    }

    subplot->base.render  = Subplot_Render;
    subplot->base.destroy = Subplot_Destroy;
    subplot->base.id      = subplot_idx_count++;
    subplot->subplot_layers_count = 0;
    memset(subplot->layers, 0, sizeof(subplot->layers));
    return subplot;
}

static PlotLayer* Subplot_AddLayer(Subplot* self){
    if(self == NULL || self->subplot_layers_count >= MAX_SUBPLOT_LAYERS){
        return NULL;
    }
    PlotLayer* layer = PlotLayer_Create();
    if(layer == NULL) return NULL;
    self->layers[self->subplot_layers_count++] = layer;
    return layer;
}

int Subplot_AddPlot(Subplot* self, plot* p){
    PlotLayer* layer = Subplot_AddLayer(self);
    if(layer == NULL) return -1;

    PlotLayer_SetData(layer, p);

    Frame_t f = {
        .parent = &self->base.frame,
    };
    Object_SetFrame((Object*)layer, &f);
    return 0;
}

/*
|----------Subplot----------|
|                           |
|    |------Layer------|    |
|    |                 |    |
|  y |                 |    |
|    |_________________|    |
|            x              |
|                           |
|          Title            |
|___________________________|
*/
void Subplot_Update(Subplot* self){
    if(self == NULL) return;

    self->viewport->x = (int)self->base.frame.Hom.t.u + 2;
    self->viewport->y = (int)self->base.frame.Hom.t.v + 2;
    self->viewport->w = self->base.width  - 4;
    self->viewport->h = self->base.height - 4;

    for(int i = 0; i < self->subplot_layers_count; i++){
        if(self->layers[i] != NULL){
            Frame_t f = {
                .parent = &self->base.frame,
                .Hom = {
                    .R = { 1, 0, 0, 1 },
                    .t = {
                        .u = self->viewport->w * SUBPLOT_H_MARGIN_RATIO,
                        .v = self->viewport->h * SUBPLOT_V_MARGIN_RATIO,
                    }
                },
            };
            Object_SetSize((Object*)self->layers[i],
                           (int32_t)(self->viewport->w * (1 - 2 * SUBPLOT_H_MARGIN_RATIO)),
                           (int32_t)(self->viewport->h * (1 - 2 * SUBPLOT_V_MARGIN_RATIO)));
            Object_SetFrame((Object*)self->layers[i], &f);
        }
    }
}
