#ifndef FIGURE_H
#define FIGURE_H

#include <SDL2/SDL.h>
#include "common_types.h"
#include "Object.h"


#define MAX_PLOTS 100
#define MAX_OBJECTS 128

#define FIGURE_MIN_WIDTH 320
#define FIGURE_MIN_HEIGHT 240

typedef struct Figure Figure;

struct Figure{
    SDL_Window* window;
    SDL_Renderer* renderer;
    Object* objects[MAX_OBJECTS];
    int object_counter;
    void* layout;
};


Figure* Figure_Create(const char* title);
void Figure_plot(Figure* self, plot* plt);
void Figure_Update(Figure* figure);
void Figure_Show(Figure* figure);
int Figure_add_object(Figure* figure, Object* obj);
int Figure_add_subplot(Figure* figure);
void Figure_update_layout(Figure* figure);

#endif // FIGURE_H