#ifndef __PID_H_
#define __PID_H_
#include "ti_msp_dl_config.h"  

typedef struct
{
    float kp;
    float ki;
    float kd;
    float sp;
    uint64_t t_k_1;
    float err_k_1;
    float err_int_k_1;
    
}PID_Tpyedef;
void PID_Init(PID_Tpyedef *PIDx,float kp,float ki,float kd);
void PID_ChangeSP(PID_Tpyedef *PIDx,float sp);
#endif