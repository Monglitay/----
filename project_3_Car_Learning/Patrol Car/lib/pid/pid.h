#ifndef __PID_H_
#define __PID_H_
#include "ti_msp_dl_config.h"  
#include "pid.h"
typedef struct
{
    float kp;
    float ki;
    float kd;
    float sp;
    float err;
    float co;
    float last_err;
    float last2_err;

}PID_Tpyedef;
void PID_Init(PID_Tpyedef *PIDx,float kp,float ki,float kd);
void PID_Limmit(float *co,float Upper,float Lower);

void PID_ChangeSP(PID_Tpyedef *PIDx,float sp);
float PID_Compute(PID_Tpyedef *PIDx,float fb);
void PID_Reset(PID_Tpyedef *PIDx);

#endif