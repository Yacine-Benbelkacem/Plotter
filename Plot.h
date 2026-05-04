#ifndef PLOT_H
#define PLOT_H

typedef struct point{
    float x;
    float y;
}point;

typedef struct plot{
    point* points;
    point  displayed_points[10000];
    int num_points;
    int num_points_displayed;
    float y_min_displayed_point;
    float y_max_displayed_point; 
}plot;

plot* plot_init(int num_points);
plot* plot_add_point(plot* p, float x, float y);
void plot_set_nb_points_to_display(plot* p, int nb_points_to_display);
void plot_update(void* self);

#endif // PLOT_H