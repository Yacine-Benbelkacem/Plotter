#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Frame.h"


MatH Frame_multH(MatH r0, MatH r1){
    MatH res = {
        .R = {  .rxx = r0.R.rxx*r1.R.rxx + r0.R.rxy*r1.R.ryx,
                .rxy = r0.R.rxx*r1.R.rxy + r0.R.rxy*r1.R.ryy,
                .ryx = r0.R.ryx*r1.R.rxx + r0.R.ryy*r1.R.ryx,
                .ryy = r0.R.ryx*r1.R.rxy + r0.R.ryy*r1.R.ryy},
        .t = {  .u = r0.R.rxx*r1.t.u+r0.R.rxy*r1.t.v+r0.t.u*r1.t.s,
                .v = r0.R.ryx*r1.t.u+r0.R.ryy*r1.t.v+r0.t.v*r1.t.s,
                .s = r0.t.s * r1.t.s},
    };
    return res;
}

Vect Frame_multHV(MatH r0, Vect v){
    Vect res = {
        .u = (r0.R.rxx*v.u + r0.R.rxy*v.v) + r0.t.u*v.s,
        .v = (r0.R.ryx*v.u + r0.R.ryy*v.v) + r0.t.v*v.s,
        .s = r0.t.s * v.s,
    };
    return res;
}



int Frame_SetOrigin(Frame_t* frame, double u, double v){
    frame->Hom.t.u = u;
    frame->Hom.t.v = v;
    return 0;
}

int Frame_SetRotation(Frame_t* frame, double * Rot){
    frame->Hom.R.rxx = Rot[0];
    frame->Hom.R.rxy = Rot[1];
    frame->Hom.R.ryx = Rot[2];
    frame->Hom.R.ryy = Rot[3];
    if((frame->Hom.R.rxx == 0 && frame->Hom.R.ryx == 0) ||
       (frame->Hom.R.ryx == 0 && frame->Hom.R.ryy == 0)){
        frame->Hom.R.rxx = 1;
        frame->Hom.R.ryy = 1;
    }
    return 0;
}

int Frame_SetScale(Frame_t* frame, double s){
    frame->Hom.t.s = s == 0 ? 1 : s; // Avoid zero scale
    return 0;
}

int Frame_pInit(Frame_t** frame_ptr){
    Frame_t* frame = (Frame_t*)malloc(sizeof(Frame_t));
    if(frame == NULL){
        return -1;
    }
    frame->parent = NULL;
    frame->label[0] = '\0'; // Initialize label to empty string
    frame->Hom.t.u = 0;
    frame->Hom.t.v = 0;
    frame->Hom.t.s = 1;
    frame->Hom.R.rxx = 1; 
    frame->Hom.R.rxy = 0;
    frame->Hom.R.ryx = 0;   
    frame->Hom.R.ryy = 1;

    *frame_ptr = frame;
    return 0;
}

Frame_t* Frame_Init(Frame_t* parent,
                    char* label, 
                    double u,
                    double v,
                    double s,
                    double rxx, double rxy,
                    double ryx, double ryy){
    Frame_t* frame = (Frame_t*)malloc(sizeof(Frame_t));
    if(frame == NULL){
        return NULL;
    }
    Frame_SetOrigin(frame, u, v);
    double rot[4] = {rxx, rxy, ryx, ryy};
    Frame_SetRotation(frame, rot);
    Frame_SetScale(frame, s);
    frame->parent = parent;
    if(label != NULL){
        strncpy(frame->label, label, sizeof(frame->label) - 1);
        frame->label[sizeof(frame->label) - 1] = '\0';
    }else{
        strcpy(frame->label, "None"); // Set to empty string if label is NULL';
    }


    return frame;
}

void printFrame(Frame_t* frame){
    printf("Frame:\n");
    printf("  Label: %s\n", (frame->label[0] == '\0') ? "None" : frame->label);
    printf("  Parent: %s\n",(frame->parent==NULL) ? "None" : ((Frame_t*)frame->parent)->label);
    printf("  Origin: (%.2f, %.2f)\n", frame->Hom.t.u, frame->Hom.t.v);
    printf("  Scale: %.2f\n", frame->Hom.t.s);
    printf("  Rotation:\n");
    printf("    rxx: %.2f, rxy: %.2f\n", frame->Hom.R.rxx, frame->Hom.R.rxy);
    printf("    ryx: %.2f, ryy: %.2f\n", frame->Hom.R.ryx, frame->Hom.R.ryy);
}

void Frame_Destroy(Frame_t* frame){
    if(frame != NULL){
        free(frame);
    }
}