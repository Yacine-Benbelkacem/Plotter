#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "Object.h"
#include "Frame.h"


static int32_t id_counter = 0;


Object* Object_Init(int32_t width,
                    int32_t height,
                    Frame_t* frame)
{
    Object* obj = malloc(sizeof(Object));
    if(obj == NULL){
        return NULL;
    }

    *obj = (Object){
        .id = id_counter++,
        .width = width,
        .height = height,
        .frame = *frame
    };

    return obj;
}

int Object_SetSize(Object* object, int32_t width, int32_t height)
{
    if(object != NULL)
    {
        object->width = width;
        object->height = height;
        return 0;
    }
    return -1;
}

int Object_SetFrame(Object* obj, Frame_t* frame){
    if((obj != NULL)
        && (frame != NULL)){
        obj->frame = *frame;
        return 0;
    }
    return -1;
}

void Object_Destroy(Object* obj){
    if(obj != NULL){
        obj->destroy(obj);
    }
    free(obj);
}

void print_object(Object* obj){
    if(obj != NULL){
        printf("Object ID: %d\n", obj->id);
        printf("Width: %d, Height: %d\n", obj->width, obj->height);
        printf("Frame Origin: (%.2f, %.2f), Scale: %.2f\n",
               obj->frame.Hom.t.u, obj->frame.Hom.t.v, obj->frame.Hom.t.s);
        printf("Frame Rotation:\n");
        printf("  rxx: %.2f, rxy: %.2f\n", obj->frame.Hom.R.rxx, obj->frame.Hom.R.rxy);
        printf("  ryx: %.2f, ryy: %.2f\n", obj->frame.Hom.R.ryx, obj->frame.Hom.R.ryy);
    }
}