#ifndef PLOT_H
#define PLOT_H

typedef struct point{
    float x;
    float y;
}point;

typedef struct plot{
    point* points;
    int data_legnth;
}plot;

plot* plot_init(int data_legnth);

#endif // PLOT_H