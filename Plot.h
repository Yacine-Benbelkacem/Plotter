#ifndef PLOT_H
#define PLOT_H

#include <SDL2/SDL.h>
#include <stdint.h>
#include "Object.h"

#define MAX_DISPLAYABLE_POINTS 16384

typedef struct point{
    double x;
    double y;
}point;


typedef struct plot{
    Object base;
    point* points;
    int32_t current_points_count;
    int32_t points_maxcount;
    int32_t resolution;
    point  displayed_points[MAX_DISPLAYABLE_POINTS];
    int32_t displayed_points_count;
    /* Full extent of the currently displayed data (data coordinates). */
    double y_min_displayed_point;
    double y_max_displayed_point;
    double x_min_displayed_point;
    double x_max_displayed_point;
    /* Visible window in data coordinates. Equals the data extent until the
       user zooms; the data->pixel transform maps this range onto the plot. */
    double x_view_min;
    double x_view_max;
    double y_view_min;
    double y_view_max;
    int    view_initialized;
}plot;

plot* plot_init(int32_t points_maxcount);
plot* plot_insert_point(plot* p, double x, double y);

void plot_set_displayable_points_count(plot* p, int32_t nb_points_to_display);
void plot_update(void* self);

/* Zoom the visible window by `factor` (<1 zooms in, >1 out) about the given
   data-space centre. */
void plot_zoom(plot* p, double x_center, double y_center, double factor);
/* Reset the visible window to the full data extent. */
void plot_reset_view(plot* p);

#endif // PLOT_H