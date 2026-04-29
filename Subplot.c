#include "Subplot.h"
#include "Frame.h"
int subplot_idx_count=0;

void Subplot_Render(void* self, void* renderer){
    
    Subplot* subplot = (Subplot*)self;
    SDL_Renderer* rend = (SDL_Renderer*)renderer;

    // subplot base updated by Figure layout.
    subplot->viewport->x = subplot->base.frame.Hom.t.u+1;
    subplot->viewport->y = subplot->base.frame.Hom.t.v+1;
    
    subplot->viewport->w = subplot->base.width-1;
    subplot->viewport->h = subplot->base.height-1;

    SDL_RenderSetViewport(rend, subplot->viewport);
    // Render the viewport
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255); // White background
    SDL_RenderFillRect(rend, NULL);
    // Render layers (not implemented yet)
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