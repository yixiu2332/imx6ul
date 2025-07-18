#ifndef _BSP_EXTI_H
#define _BSP_EXTI_H

#include "imx6ul.h"
#include "bsp_gpio.h"
#include "bsp_int.h"

/* 函数声明 */
void exti_init(void);						/* 中断初始化 */
void gpio1_io18_irqhandler(void);
#endif
