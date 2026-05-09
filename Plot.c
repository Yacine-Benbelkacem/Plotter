#include "Plot.h"
#include <stdlib.h>
#include <stdio.h>

#define X_AXIS 0
#define Y_AXIS 1

#include <SDL2/SDL.h>
#include "Object.h"

void CurveLayer_Data2Pixel(plot* data){
    for(int i = 0; i < data->displayed_points_count; i++){

        MatH tr = Frame_GetTransform(&data->base.frame);

        Vect res = Frame_multHV(
                    tr, 
                    (Vect){.u = data->displayed_points[i].x,
                            .v = data->displayed_points[i].y,
                            .s = 1});
        data->displayed_points[i].x = res.u;
        data->displayed_points[i].y = res.v;
    }
}

void render(void * self, void * renderer){
        CurveLayer_Data2Pixel((plot*) self);
        SDL_SetRenderDrawColor(renderer,255,0,0,255);
        for(int i=0; i< ((plot*) self)->displayed_points_count-1; i++){
            SDL_RenderDrawLine((SDL_Renderer*)renderer, 
                               ((plot*)self)->displayed_points[i].x,
                               ((plot*)self)->displayed_points[i].y,
                               ((plot*)self)->displayed_points[i+1].x,
                               ((plot*)self)->displayed_points[i+1].y);
        }

}

double get_max(point array[], int32_t size, int32_t dim)
{
    double max = ((double*)(&array[0]))[dim];

    for(int32_t i = 0; i < size; i++){
        if(((double*)(&array[i]))[dim] > max){
            max = ((double*)(&array[i]))[dim];
        }
    }
 
    return max;
}

double get_min(point array[], int32_t size, int32_t dim)
{
    double min = ((double*)(&array[0]))[dim];

    for(int32_t i=0; i< size; i++){
        if(min > ((double*)(&array[i]))[dim]){
            min = ((double*)(&array[i]))[dim];
        }
    }

    return min;
}

plot* plot_init(int32_t points_count)
{
    plot* p = (plot*)malloc(sizeof(plot));
    p->points = (point*)malloc(points_count * sizeof(point));
    if(!p->points){
        free(p);
        return NULL;
    }
    p->points_count = points_count;
    p->displayed_points_count = points_count;
    p->current_point_idx = 0;
    p->resolution = 1;

    p->base.render = render;
    return p;
}

static void resample(plot * plt){
    // Implement resampling logic here
    if( plt->displayed_points_count < plt->current_point_idx){
        for(int32_t i = 0; i < plt->displayed_points_count; i++){
            int32_t index = i * (plt->current_point_idx / plt->displayed_points_count);
            plt->displayed_points[i] = plt->points[index];
        }
    }else{
        for(int32_t i = 0; i < plt->current_point_idx; i++){
            plt->displayed_points[i] = plt->points[i];
        }
        plt->displayed_points_count = plt->current_point_idx;
    }
}

void plot_update(void* self){
        plot* plt = (plot*)self;
        if(plt != NULL){
            // zoom logic (future feature)
            resample(plt);
            plt->y_max_displayed_point = get_max(plt->displayed_points, plt->displayed_points_count,Y_AXIS);
            plt->y_min_displayed_point = get_min(plt->displayed_points, plt->displayed_points_count,Y_AXIS);
            plt->x_max_displayed_point = get_max(plt->displayed_points, plt->displayed_points_count,X_AXIS);
            plt->x_min_displayed_point = get_min(plt->displayed_points, plt->displayed_points_count,X_AXIS);
            
        }
}

void plot_set_nb_points_to_display(plot* p, int32_t nb_points_to_display){
    if(p != NULL && nb_points_to_display <= p->points_count){
        p->displayed_points_count = nb_points_to_display;
    }
}

plot* plot_add_point(plot* p, double x, double y){
    if(p && p->points && p->points_count > 0){
        if(p->current_point_idx < p->points_count){
            p->points[p->current_point_idx].x = x;
            p->points[p->current_point_idx].y = y;
            p->current_point_idx++;
            return p;
        }
    }else{
        plot * p =plot_init(1);
        return(plot_add_point(p, x, y));
    }
}