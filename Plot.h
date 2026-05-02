#ifndef PLOT_H
#define PLOT_H

typedef struct point{
    float x;
    float y;
}point;

typedef struct plot{
    point* points;
    point  resampled_points[10000];
    int num_points;
    int num_points_displayed;
}plot;

plot* plot_init(int num_points);
plot* plot_add_point(plot* p, float x, float y);
void plot_set_nb_points_to_display(plot* p, int nb_points_to_display);

#endif // PLOT_H