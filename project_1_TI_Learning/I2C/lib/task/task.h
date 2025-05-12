#include "delay.h"

#define PERIODIC(T) \
static uint32_t nxt = 0; \
if(get_ticks() < nxt) return; \
nxt += (T);

#define PERIODIC_START(NAME, T) \
static uint32_t NAME##_nxt = 0; \
if(get_ticks() >= NAME##_nxt) {\
NAME##_nxt += (T);

#define PERIODIC_END }
