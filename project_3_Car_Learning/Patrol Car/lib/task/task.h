#include "delay.h"

/**
 * @brief ������ִ�к�
 * @param T ����ʱ�䣨��λȡ����get_ticks()������ʱ�䵥λ��ͨ���Ǻ��룩
 *
 * ʹ�÷�����
 * ����Ҫ������ִ�еĺ�����ͷʹ�ô˺ꡣ
 * �����ǰʱ��δ������һ��ִ��ʱ��㣬�������������ء�
 * ���򣬸�����һ��ִ��ʱ��㲢����ִ�к����塣
 */
#define PERIODIC(T) \
static uint32_t nxt = 0; \
if(get_ticks() < nxt) return; \
nxt += (T);

/**
 * @brief ������ִ�п鿪ʼ��
 * @param NAME Ψһ�ı�ʶ�����������ֲ�ͬ�������Կ�
 * @param T ����ʱ�䣨��λȡ����get_ticks()������ʱ�䵥λ��ͨ���Ǻ��룩
 *
 * ʹ�÷�����
 * ���ڿ�ʼһ��������ִ�еĴ���顣
 * �����ǰʱ��ﵽ�򳬹���һ��ִ��ʱ��㣬����齫��ִ�С�
 * ������ PERIODIC_END �����ʹ�á�
 */
#define PERIODIC_START(NAME, T) \
static uint32_t NAME##_nxt = 0; \
if(get_ticks() >= NAME##_nxt) {\
NAME##_nxt += (T);

/**
 * @brief ������ִ�п������
 *
 * ʹ�÷�����
 * ���ڽ����� PERIODIC_START ��ʼ��������ִ�д���顣
 */
#define PERIODIC_END }
