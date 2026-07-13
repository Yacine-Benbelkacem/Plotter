#include "Plot.h"
#include "Object.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>

#define X_AXIS 0
#define Y_AXIS 1

static void PlotLayer_Data2Pixel(plot* data){
    MatH tr = Frame_GetTransform(&data->base.frame);
    for(int i = 0; i < data->displayed_points_count; i++){
        Vect res = Frame_multHV(tr, (Vect){
            .u = data->displayed_points[i].x,
            .v = data->displayed_points[i].y,
            .s = 1
        });
        data->displayed_points[i].x = res.u;
        data->displayed_points[i].y = res.v;
    }
}

static void plot_render(void* self, void* renderer){
    PlotLayer_Data2Pixel((plot*)self);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    plot* plt = (plot*)self;
    for(int i = 0; i < plt->displayed_points_count - 1; i++){
        SDL_RenderDrawLine((SDL_Renderer*)renderer,
                           (int)plt->displayed_points[i].x,
                           (int)plt->displayed_points[i].y,
                           (int)plt->displayed_points[i + 1].x,
                           (int)plt->displayed_points[i + 1].y);
    }
}

static double get_max(point array[], int32_t size, int32_t dim){
    double max = (dim == X_AXIS) ? array[0].x : array[0].y;
    for(int32_t i = 1; i < size; i++){
        double val = (dim == X_AXIS) ? array[i].x : array[i].y;
        if(val > max) max = val;
    }
    return max;
}

static double get_min(point array[], int32_t size, int32_t dim){
    double min = (dim == X_AXIS) ? array[0].x : array[0].y;
    for(int32_t i = 1; i < size; i++){
        double val = (dim == X_AXIS) ? array[i].x : array[i].y;
        if(val < min) min = val;
    }
    return min;
}

plot* plot_init(int32_t points_count){
    plot* p = malloc(sizeof(plot));
    if(!p) return NULL;
    p->points = malloc(points_count * sizeof(point));
    if(!p->points){
        free(p);
        return NULL;
    }
    p->points_maxcount   = points_count;
    p->displayed_points_count = points_count;
    p->current_points_count   = 0;
    p->resolution        = 1;
    p->base.render       = plot_render;
    p->base.destroy      = NULL;
    return p;
}

static void resample(plot* plt){
    if(plt->displayed_points_count < plt->current_points_count){
        int32_t step = plt->current_points_count / plt->displayed_points_count;
        for(int32_t i = 0; i < plt->displayed_points_count; i++){
            plt->displayed_points[i] = plt->points[i * step];
        }
    } else {
        for(int32_t i = 0; i < plt->current_points_count; i++){
            plt->displayed_points[i] = plt->points[i];
        }
        plt->displayed_points_count = plt->current_points_count;
    }
}

void plot_update(void* self){
    plot* plt = (plot*)self;
    if(plt == NULL) return;
    resample(plt);
    plt->y_max_displayed_point = get_max(plt->displayed_points, plt->displayed_points_count, Y_AXIS);
    plt->y_min_displayed_point = get_min(plt->displayed_points, plt->displayed_points_count, Y_AXIS);
    plt->x_max_displayed_point = get_max(plt->displayed_points, plt->displayed_points_count, X_AXIS);
    plt->x_min_displayed_point = get_min(plt->displayed_points, plt->displayed_points_count, X_AXIS);
}

void plot_set_displayable_points_count(plot* p, int32_t nb_points_to_display){
    if(p != NULL && nb_points_to_display > 0 && nb_points_to_display <= p->points_maxcount){
        p->displayed_points_count = nb_points_to_display;
    }
}

plot* plot_insert_point(plot* p, double x, double y){
    if(!p || !p->points || p->points_maxcount <= 0)
        return NULL;
    if(p->current_points_count < p->points_maxcount){
        p->points[p->current_points_count].x = x;
        p->points[p->current_points_count].y = y;
        p->current_points_count++;
    }
    return p;
}
