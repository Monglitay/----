#include "ti_msp_dl_config.h"
#include "mspm0_clock.h"

volatile unsigned long tick_ms;
volatile uint32_t start_time;
int mspm0_delay_ms(unsigned long num_ms)
{
    num_ms=num_ms*32000;
    delay_cycles(num_ms);
    return 0;
}

int mspm0_get_clock_ms(unsigned long *count)
{
    if (!count)
        return 1;
    count[0] = tick_ms;
    return 0;
}
int get_ticks()
{
    return tick_ms;
}
void SysTick_Handler(void)
{
    tick_ms++;
}