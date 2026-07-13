#include "Subplot.h"
#include "Frame.h"
#include "PlotLayer.h"
#include "Text.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define MAX_SUBPLOT_LAYERS 10
#define AXIS_TICK_TARGET    6   /* desired number of ticks per axis */
#define AXIS_TICK_LEN       5   /* tick mark length in pixels */

static int subplot_idx_count = 0;

static const SDL_Color COLOR_AXIS  = { 60,  60,  60, 255};
static const SDL_Color COLOR_GRID  = {225, 225, 225, 255};
static const SDL_Color COLOR_TEXT  = { 30,  30,  30, 255};

/* Round `range` to a "nice" value (1, 2, 5 x 10^k). When `round` is set the
   nearest nice number is returned, otherwise the smallest nice number >= range. */
static double nice_num(double range, int round){
    if(range <= 0) return 1.0;
    double expv = floor(log10(range));
    double f = range / pow(10, expv);
    double nf;
    if(round){
        if(f < 1.5)      nf = 1;
        else if(f < 3)   nf = 2;
        else if(f < 7)   nf = 5;
        else             nf = 10;
    } else {
        if(f <= 1)       nf = 1;
        else if(f <= 2)  nf = 2;
        else if(f <= 5)  nf = 5;
        else             nf = 10;
    }
    return nf * pow(10, expv);
}

/* Draw the frame, gridlines, ticks, numeric labels and axis/plot titles.
   `area` is the plot rectangle in the current (viewport-local) coordinates. */
static void draw_axes(SDL_Renderer* r, SDL_Rect area, const Subplot* sp,
                      const plot* ref){
    double xmin = ref->x_view_min, xmax = ref->x_view_max;
    double ymin = ref->y_view_min, ymax = ref->y_view_max;
    if(xmax <= xmin || ymax <= ymin) return;

    char buf[32];

    /* X ticks + vertical gridlines. */
    double xstep = nice_num(nice_num(xmax - xmin, 0) / (AXIS_TICK_TARGET - 1), 1);
    for(double v = ceil(xmin / xstep) * xstep; v <= xmax + xstep * 0.5; v += xstep){
        int px = area.x + (int)((v - xmin) / (xmax - xmin) * area.w + 0.5);
        if(px < area.x || px > area.x + area.w) continue;
        SDL_SetRenderDrawColor(r, COLOR_GRID.r, COLOR_GRID.g, COLOR_GRID.b, 255);
        SDL_RenderDrawLine(r, px, area.y, px, area.y + area.h);
        SDL_SetRenderDrawColor(r, COLOR_AXIS.r, COLOR_AXIS.g, COLOR_AXIS.b, 255);
        SDL_RenderDrawLine(r, px, area.y + area.h, px, area.y + area.h + AXIS_TICK_LEN);
        snprintf(buf, sizeof(buf), "%g", v);
        Text_Draw(r, buf, px, area.y + area.h + AXIS_TICK_LEN + 9, TEXT_ALIGN_CENTER, COLOR_TEXT);
    }

    /* Y ticks + horizontal gridlines. */
    double ystep = nice_num(nice_num(ymax - ymin, 0) / (AXIS_TICK_TARGET - 1), 1);
    for(double v = ceil(ymin / ystep) * ystep; v <= ymax + ystep * 0.5; v += ystep){
        int py = area.y + (int)((ymax - v) / (ymax - ymin) * area.h + 0.5);
        if(py < area.y || py > area.y + area.h) continue;
        SDL_SetRenderDrawColor(r, COLOR_GRID.r, COLOR_GRID.g, COLOR_GRID.b, 255);
        SDL_RenderDrawLine(r, area.x, py, area.x + area.w, py);
        SDL_SetRenderDrawColor(r, COLOR_AXIS.r, COLOR_AXIS.g, COLOR_AXIS.b, 255);
        SDL_RenderDrawLine(r, area.x - AXIS_TICK_LEN, py, area.x, py);
        snprintf(buf, sizeof(buf), "%g", v);
        Text_Draw(r, buf, area.x - AXIS_TICK_LEN - 4, py, TEXT_ALIGN_RIGHT, COLOR_TEXT);
    }

    /* Plot frame. */
    SDL_SetRenderDrawColor(r, COLOR_AXIS.r, COLOR_AXIS.g, COLOR_AXIS.b, 255);
    SDL_RenderDrawRect(r, &area);

    /* Titles. */
    if(sp->title[0] != '\0')
        Text_Draw(r, sp->title, area.x + area.w / 2, AXIS_MARGIN_TOP / 2,
                  TEXT_ALIGN_CENTER, COLOR_TEXT);
    if(sp->x_label[0] != '\0')
        Text_Draw(r, sp->x_label, area.x + area.w / 2, area.y + area.h + AXIS_MARGIN_BOTTOM - 8,
                  TEXT_ALIGN_CENTER, COLOR_TEXT);
    if(sp->y_label[0] != '\0')
        Text_DrawRotated(r, sp->y_label, area.x - AXIS_MARGIN_LEFT + 12, area.y + area.h / 2,
                         -90.0, COLOR_TEXT);
}

void Subplot_Render(void* self, void* renderer){
    Subplot* subplot = (Subplot*)self;
    SDL_Renderer* rend = (SDL_Renderer*)renderer;

    Subplot_Update(subplot);

    SDL_RenderSetViewport(rend, subplot->viewport);
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    SDL_RenderFillRect(rend, NULL);

    subplot->base.frame.Hom.t.u = 0;
    subplot->base.frame.Hom.t.v = 0;

    /* Plot rectangle in viewport-local coordinates. */
    SDL_Rect area = {
        .x = AXIS_MARGIN_LEFT,
        .y = AXIS_MARGIN_TOP,
        .w = subplot->viewport->w - AXIS_MARGIN_LEFT - AXIS_MARGIN_RIGHT,
        .h = subplot->viewport->h - AXIS_MARGIN_TOP - AXIS_MARGIN_BOTTOM,
    };

    plot* ref = (subplot->subplot_layers_count > 0) ? subplot->layers[0]->data : NULL;
    if(ref != NULL && ref->view_initialized){
        draw_axes(rend, area, subplot, ref);
    }

    /* Clip data drawing to the plot rectangle so zoomed traces do not spill
       over the axes and labels. */
    SDL_RenderSetClipRect(rend, &area);
    for(int i = 0; i < subplot->subplot_layers_count; i++){
        if(subplot->layers[i] != NULL){
            ((Object*)subplot->layers[i])->render(subplot->layers[i], renderer);
        }
    }
    SDL_RenderSetClipRect(rend, NULL);
}

void Subplot_Destroy(void* subplot_ptr){
    Subplot* subplot = (Subplot*)subplot_ptr;
    if(subplot == NULL) return;

    if(subplot->viewport != NULL){
        free(subplot->viewport);
    }
    for(int i = 0; i < subplot->subplot_layers_count; i++){
        if(subplot->layers[i] != NULL){
            Object* obj = (Object*)subplot->layers[i];
            if(obj->destroy != NULL){
                obj->destroy(subplot->layers[i]);
            } else {
                free(subplot->layers[i]);
            }
        }
    }
    free(subplot);
}

Subplot* Subplot_Create(){
    Subplot* subplot = malloc(sizeof(Subplot));
    if(subplot == NULL) return NULL;

    subplot->viewport = malloc(sizeof(SDL_Rect));
    if(subplot->viewport == NULL){
        free(subplot);
        return NULL;
    }

    subplot->base.render  = Subplot_Render;
    subplot->base.destroy = Subplot_Destroy;
    subplot->base.id      = subplot_idx_count++;
    subplot->subplot_layers_count = 0;
    subplot->title[0]   = '\0';
    subplot->x_label[0] = '\0';
    subplot->y_label[0] = '\0';
    memset(&subplot->plot_area, 0, sizeof(subplot->plot_area));
    memset(subplot->layers, 0, sizeof(subplot->layers));
    return subplot;
}

void Subplot_SetTitle(Subplot* self, const char* title){
    if(self && title) snprintf(self->title, sizeof(self->title), "%s", title);
}
void Subplot_SetXLabel(Subplot* self, const char* label){
    if(self && label) snprintf(self->x_label, sizeof(self->x_label), "%s", label);
}
void Subplot_SetYLabel(Subplot* self, const char* label){
    if(self && label) snprintf(self->y_label, sizeof(self->y_label), "%s", label);
}

static PlotLayer* Subplot_AddLayer(Subplot* self){
    if(self == NULL || self->subplot_layers_count >= MAX_SUBPLOT_LAYERS){
        return NULL;
    }
    PlotLayer* layer = PlotLayer_Create();
    if(layer == NULL) return NULL;
    self->layers[self->subplot_layers_count++] = layer;
    return layer;
}

int Subplot_AddPlot(Subplot* self, plot* p){
    PlotLayer* layer = Subplot_AddLayer(self);
    if(layer == NULL) return -1;

    PlotLayer_SetData(layer, p);

    Frame_t f = {
        .parent = &self->base.frame,
    };
    Object_SetFrame((Object*)layer, &f);
    return 0;
}

int Subplot_ZoomAt(Subplot* self, int mx, int my, double factor){
    if(self == NULL) return 0;
    SDL_Rect a = self->plot_area;
    if(a.w <= 0 || a.h <= 0) return 0;
    if(mx < a.x || mx > a.x + a.w || my < a.y || my > a.y + a.h) return 0;

    for(int i = 0; i < self->subplot_layers_count; i++){
        if(self->layers[i] == NULL) continue;
        plot* d = self->layers[i]->data;
        if(d == NULL || !d->view_initialized) continue;
        double dx = d->x_view_min + (double)(mx - a.x) / a.w * (d->x_view_max - d->x_view_min);
        double dy = d->y_view_max - (double)(my - a.y) / a.h * (d->y_view_max - d->y_view_min);
        plot_zoom(d, dx, dy, factor);
    }
    return 1;
}

/*
|----------Subplot----------|
|          Title            |
|    |------Layer------|    |
|  y |                 |    |
|    |_________________|    |
|            x              |
|___________________________|
*/
void Subplot_Update(Subplot* self){
    if(self == NULL) return;

    self->viewport->x = (int)self->base.frame.Hom.t.u + 2;
    self->viewport->y = (int)self->base.frame.Hom.t.v + 2;
    self->viewport->w = self->base.width  - 4;
    self->viewport->h = self->base.height - 4;

    int32_t area_w = self->viewport->w - AXIS_MARGIN_LEFT - AXIS_MARGIN_RIGHT;
    int32_t area_h = self->viewport->h - AXIS_MARGIN_TOP - AXIS_MARGIN_BOTTOM;

    /* Plot rectangle in window coordinates (used for zoom hit-testing). */
    self->plot_area.x = self->viewport->x + AXIS_MARGIN_LEFT;
    self->plot_area.y = self->viewport->y + AXIS_MARGIN_TOP;
    self->plot_area.w = area_w;
    self->plot_area.h = area_h;

    for(int i = 0; i < self->subplot_layers_count; i++){
        if(self->layers[i] != NULL){
            Frame_t f = {
                .parent = &self->base.frame,
                .Hom = {
                    .R = { 1, 0, 0, 1 },
                    .t = {
                        .u = AXIS_MARGIN_LEFT,
                        .v = AXIS_MARGIN_TOP,
                    }
                },
            };
            Object_SetSize((Object*)self->layers[i], area_w, area_h);
            Object_SetFrame((Object*)self->layers[i], &f);
        }
    }
}
