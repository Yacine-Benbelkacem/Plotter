#ifndef PLOT_H
#define PLOT_H

#include <stdint.h>

#define MAX_DISPLAYABLE_POINTS 16384

typedef struct point{
    double x;
    double y;
}point;


typedef struct plot{
    point* points;
    int32_t current_point_idx;
    int32_t points_count;
    int32_t resolution;
    point  displayed_points[MAX_DISPLAYABLE_POINTS];
    int32_t displayed_points_count;
    double y_min_displayed_point;
    double y_max_displayed_point; 
    double x_min_displayed_point;
    double x_max_displayed_point; 

}plot;

plot* plot_init(int32_t points_count);
plot* plot_add_point(plot* p, double x, double y);

void plot_set_nb_points_to_display(plot* p, int32_t nb_points_to_display);
void plot_update(void* self);

#endif // PLOT_H