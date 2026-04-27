#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>
#include "Frame.h"

typedef struct Object{
    int32_t  id;
    int32_t  width;
    int32_t  height;
    Frame_t  frame;

    void(*render)(void* self, void* renderer);
}Object;

Object* Object_Init(int32_t* width,
                    int32_t* height,
                    Frame_t* frame);
int Object_SetFrame(Object* obj, Frame_t* frame);

void print_object(Object* obj);
void Object_Destroy(Object* obj);

#endif // OBJECT_H