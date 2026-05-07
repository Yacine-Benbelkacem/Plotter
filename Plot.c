#include "Plot.h"
#include <stdlib.h>
#include <stdio.h>

#define X_AXIS 0
#define Y_AXIS 1



float get_max(point array[], int size, int dim){

    float max = ((float*)(&array[0]))[dim];


    for(int i = 0; i < size; i++){
        if(((float*)(&array[i]))[dim] > max){
            max = ((float*)(&array[i]))[dim];
        }
    }
    return max;
}

float get_min(point array[], int size, int dim){
    float min = ((float*)(&array[0]))[dim];
    for(int i=0; i< size; i++){
        if(min > ((float*)(&array[i]))[dim]){
            min = ((float*)(&array[i]))[dim];
        }
    }
    return min;
}

plot* plot_init(int points_count){
    plot* p = (plot*)malloc(sizeof(plot));
    p->points = (point*)malloc(points_count * sizeof(point));
    if(!p->points){
        free(p);
        return NULL;
    }
    p->points_count = points_count;
    p->displayed_points_count = points_count;
    p->current_point_idx = 0;
    return p;
}

void resample(plot * plt){
    // Implement resampling logic here
    if( plt->displayed_points_count < plt->current_point_idx){
        for(int i = 0; i < plt->displayed_points_count; i++){
            int index = i * (plt->current_point_idx / plt->displayed_points_count);
            plt->displayed_points[i] = plt->points[index];
        }
    }else{
        for(int i = 0; i < plt->current_point_idx; i++){
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

void plot_set_nb_points_to_display(plot* p, int nb_points_to_display){
    if(p != NULL && nb_points_to_display <= p->points_count){
        p->displayed_points_count = nb_points_to_display;
    }
}

plot* plot_add_point(plot* p, float x, float y){
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