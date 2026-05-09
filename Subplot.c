#include "Subplot.h"
#include "Frame.h"
#include "CurveLayer.h"

#define MAX_SUBPLOT_LAYERS 10

int subplot_idx_count=0;

void Subplot_Render(void* self, void* renderer){
    
    Subplot* subplot = (Subplot*)self;
    SDL_Renderer* rend = (SDL_Renderer*)renderer;

    Subplot_Update(subplot);

    // This instruction switches frame reference to subplot frame
    SDL_RenderSetViewport(rend, subplot->viewport);
    // Render the viewport
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255); // White background
    SDL_RenderFillRect(rend, NULL);
    // Render layers with respect to subplot frame
    for(int i = 0; i < subplot->subplot_layers_count; i++){
        if(subplot->layers[i] != NULL){
            ((Object*)subplot->layers[i])->render(subplot->layers[i], renderer);
        }
    }
}


void Subplot_Destroy(void* subplot_ptr){

    Subplot* subplot = (Subplot*)subplot_ptr;
    if(subplot != NULL){
        if(subplot->viewport != NULL){
            free(subplot->viewport);
        }
        for(int i = 0; i < subplot->subplot_layers_count; i++){
            if(subplot->layers[i] != NULL){
                free(subplot->layers[i]);
            }
        }
        free(subplot);
    }
}

Subplot* Subplot_Create(){
    Subplot* subplot = (Subplot*)malloc(sizeof(Subplot));
    if(subplot == NULL){
        return NULL;
    }
    subplot->viewport = (SDL_Rect*)malloc(sizeof(SDL_Rect));
    if(subplot->viewport == NULL){
        free(subplot);
        return NULL;
    }

    
    subplot->base.render = Subplot_Render;
    subplot->base.id = subplot_idx_count++;
    subplot->subplot_layers_count = 0;

    memset(subplot->layers, 0, sizeof(subplot->layers));
    subplot->base.destroy = Subplot_Destroy;
    return subplot;
}

CurveLayer* Subplot_AddLayer(Subplot* self){
    if(self == NULL || self->subplot_layers_count >= MAX_SUBPLOT_LAYERS){
        return NULL;
    }
    CurveLayer* layer = CurveLayer_Create();
    if(layer == NULL){
        return NULL;
    }
    self->layers[self->subplot_layers_count++] = layer;
    return layer;
}

int Subplot_AddPlot(Subplot* self, plot* p){
    CurveLayer* layer =  Subplot_AddLayer(self);
    CurveLayer_SetData(layer,p);
    
    Frame_t f = {
        .parent = &self->base.frame,
    };

    Object_SetFrame((Object*) layer, &f);
}

void Subplot_Update(Subplot* self){

    // subplot base updated by Figure layout.
    self->viewport->x = self->base.frame.Hom.t.u+2;
    self->viewport->y = self->base.frame.Hom.t.v+2;
    
    self->viewport->w = self->base.width-4;
    self->viewport->h = self->base.height-4;

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

    if(self != NULL){
        for(int i = 0; i < self->subplot_layers_count; i++){
            if(self->layers[i] != NULL){
                Object_SetOrigin((Object*)(&(*self->layers[i])), SUBPLOT_H_MARGIN, SUBPLOT_V_MARGIN);
                Object_SetSize((Object*)(&(*self->layers[i])), self->viewport->w-2*SUBPLOT_H_MARGIN, self->viewport->h-2*SUBPLOT_V_MARGIN);
            }
        }
    }   


}