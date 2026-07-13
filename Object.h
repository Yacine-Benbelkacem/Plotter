#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>
#include "Frame.h"

typedef struct Object{
    Frame_t  frame;
    int32_t  id;
    int32_t  width;
    int32_t  height;
    

    void(*render)(void* self, void* renderer);
    void (*destroy)(void* self);
}Object;

Object* Object_Init(int32_t width,
                    int32_t height,
                    Frame_t* frame);
int Object_SetFrame(Object* obj, Frame_t* frame);
int Object_SetSize(Object* obj, int32_t width, int32_t height);
int Object_SetOrigin(Object* obj, int32_t u, int32_t v);
void print_object(Object* obj);

#endif // OBJECT_H