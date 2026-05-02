#include "Plot.h"
#include <stdlib.h>

static int current_point = 0;

plot* plot_init(int num_points){
    plot* p = (plot*)malloc(sizeof(plot));
    p->points = (point*)malloc(num_points * sizeof(point));
    if(!p->points){
        free(p);
        return NULL;
    }
    p->num_points = num_points;
    return p;
}

void resample(plot * plt){
    // Implement resampling logic here
    if( plt->num_points_displayed < plt->num_points){
        for(int i = 0; i < plt->num_points; i++){
            int index = i * (plt->num_points / plt->num_points_displayed);
            plt->resampled_points[i] = plt->points[index];
        } 
    }else{
        for(int i = 0; i < plt->num_points; i++){
            plt->resampled_points[i] = plt->points[i];
        }
        plt->num_points_displayed = plt->num_points;
    }
}

void plot_update(void* self){
        plot* plt = (plot*)self;
        if(plt != NULL){
            resample(plt);
        }
        

}

void plot_set_nb_points_to_display(plot* p, int nb_points_to_display){
    if(p != NULL && nb_points_to_display <= p->num_points){
        p->num_points_displayed = nb_points_to_display;
    }
}

int plot_add_point(plot* p, float x, float y){
    if(p && p->points && p->num_points > 0){
        if(current_point < p->num_points){
            p->points[current_point].x = x;
            p->points[current_point].y = y;
            current_point++;
            return p;
        }
    }else{
        plot * p =plot_init(1);
        return(plot_add_point(p, x, y));
    }
}