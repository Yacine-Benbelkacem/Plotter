#include "Frame.h"


MatH Frame_multH(MatH r0, MatH r1){
    MatH res = {
        .R = {  .rxx = (r0.R.rxx*r1.R.rxx + r0.R.rxy*r1.R.ryx)*r0.t.s,
                .rxy = (r0.R.rxx*r1.R.rxy + r0.R.rxy*r1.R.ryy)*r0.t.s,
                .ryx = (r0.R.ryx*r1.R.rxx + r0.R.ryy*r1.R.ryx)*r0.t.s,
                .ryy = (r0.R.ryx*r1.R.rxy + r0.R.ryy*r1.R.ryy)*r0.t.s},
        .t = {  .u = r0.R.rxx*r1.t.u+r0.R.rxy*r1.t.v+r0.t.u*r1.t.s,
                .v = r0.R.ryx*r1.t.u+r0.R.ryy*r1.t.v+r0.t.v*r1.t.s,
                .s = r0.t.s * r1.t.s},
    };
    return res;
}

Vect Frame_multHV(MatH r0, Vect v){
    Vect res = {
        .u = (r0.R.rxx*v.u + r0.R.rxy*v.v)*r0.t.s + r0.t.u*v.s,
        .v = (r0.R.ryx*v.u + r0.R.ryy*v.v)*r0.t.s + r0.t.v*v.s,
        .s = r0.t.s * v.s,
    };
    return res;
}



int Frame_SetOrigin(Frame* frame, double u, double v){
    frame->Hom.t.u = u;
    frame->Hom.t.v = v;
    return 0;
}

int Frame_SetRotation(Frame* frame, double * Rot){
    frame->Hom.R.rxx = Rot[0];
    frame->Hom.R.rxy = Rot[1];
    frame->Hom.R.ryx = Rot[2];
    frame->Hom.R.ryy = Rot[3];
    return 0;
}

int Frame_SetScale(Frame* frame, double s){
    frame->Hom.t.s = s;
    return 0;
}
