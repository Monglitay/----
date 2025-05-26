#include "delay.h"

/**
 * @brief 周期性执行宏
 * @param T 周期时间（单位取决于get_ticks()函数的时间单位，通常是毫秒）
 *
 * 使用方法：
 * 在需要周期性执行的函数开头使用此宏。
 * 如果当前时间未到达下一个执行时间点，函数将立即返回。
 * 否则，更新下一个执行时间点并继续执行函数体。
 */
#define PERIODIC(T) \
static uint32_t nxt = 0; \
if(get_ticks() < nxt) return; \
nxt += (T);

/**
 * @brief 周期性执行块开始宏
 * @param NAME 唯一的标识符，用于区分不同的周期性块
 * @param T 周期时间（单位取决于get_ticks()函数的时间单位，通常是毫秒）
 *
 * 使用方法：
 * 用于开始一个周期性执行的代码块。
 * 如果当前时间达到或超过下一个执行时间点，代码块将被执行。
 * 必须与 PERIODIC_END 宏配对使用。
 */
#define PERIODIC_START(NAME, T) \
static uint32_t NAME##_nxt = 0; \
if(get_ticks() >= NAME##_nxt) {\
NAME##_nxt += (T);

/**
 * @brief 周期性执行块结束宏
 *
 * 使用方法：
 * 用于结束由 PERIODIC_START 开始的周期性执行代码块。
 */
#define PERIODIC_END }
