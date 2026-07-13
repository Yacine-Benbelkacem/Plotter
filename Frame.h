#ifndef    FRAME_H
#define    FRAME_H


typedef struct Vect2D{
    double u;
    double v;
    double s;
}Vect;

typedef struct Rot{
    double rxx;
    double rxy;
    double ryx;
    double ryy;
}Rot;

/* Matrice homogene */
typedef struct MatH{
    Rot R;
    Vect t;
}MatH;


typedef struct Frame
{
    void* parent;
    char  label[256]; // Optional label for debugging
    MatH  Hom;
}Frame_t;

int Frame_pInit(Frame_t** frame_ptr);
Frame_t* Frame_Init(Frame_t* parent,
                    char* label,
                    double u,
                    double v,
                    double s,
                    double rxx, double rxy,
                    double ryx, double ryy);
void printFrame(Frame_t* frame);

MatH Frame_multH(MatH r0, MatH r1);
Vect Frame_multHV(MatH r0, Vect v);
   
int Frame_SetOrigin(Frame_t* frame, double u, double v);
int Frame_SetRotation(Frame_t* frame, double r11, double r12, double r21, double r22);
int Frame_SetScale(Frame_t* frame, double s);
MatH Frame_GetTransform(Frame_t * frame);
void Frame_Destroy(Frame_t* frame);

#endif // FRAME_H
