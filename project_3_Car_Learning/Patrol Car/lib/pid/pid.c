#include "pid.h"
#include "delay.h"
void PID_Init(PID_Tpyedef *PIDx,float kp,float ki,float kd)
{
    PIDx->kp=kp;
    PIDx->ki=ki;
    PIDx->kd=kd;
    PIDx->err_int_k_1 =0 ;
    PIDx->err_k_1 = 0;
    PIDx->t_k_1 = 0;
}

void PID_ChangeSP(PID_Tpyedef *PIDx,float sp)
{
    PIDx->sp=sp;
}

float PID_Compute(PID_Tpyedef *PIDx,float fb)
{
    float err=PIDx->sp-fb;
    
    uint64_t t_k = get_ticks_us();
    float deltaT = (t_k - t_k-1)*1.0e-6f;
    float err_dev = (err - PIDx->err_k_1)/deltaT;
    float err_int= PIDx->err_int_k_1 + (err+PIDx->err_k_1)*deltaT*0.5f;
    float cop = PIDx->kp*err;
    float coi = PIDx->ki*err_int;
    float cod = PIDx->kd*err_dev; 
    float co = cop+coi+cod;
    PIDx->t_k_1 = t_k;
    PIDx->err_int_k_1 = err_int;
    PIDx->err_k_1 = err_dev;
    return co;
}