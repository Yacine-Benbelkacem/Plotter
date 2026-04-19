#ifndef    FRAME_H
#define    FRAME_H


typedef struct Point{
	double x;
	double y;
}Point;

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
    void * parent;
    MatH Hom;
}Frame;

MatH Frame_multH(MatH r0, MatH r1);
Vect Frame_multHV(MatH r0, Vect v);

int Frame_SetOrigin(Frame* frame, double u, double v);
int Frame_SetRotation(Frame* frame, double * Rot);
int Frame_SetScale(Frame* frame, double s);

#endif // FRAME_H
