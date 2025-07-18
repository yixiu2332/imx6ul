#ifndef __BSP_CLK_H
#define __BSP_CLK_H


#include "imx6ul.h"
/* 
 * CCM 相关寄存器地址
 */

#define CCM_CCGR0 *((volatile unsigned int *)0X020C4068)
#define CCM_CCGR1 *((volatile unsigned int *)0X020C406C)
#define CCM_CCGR2 *((volatile unsigned int *)0X020C4070)
#define CCM_CCGR3 *((volatile unsigned int *)0X020C4074)
#define CCM_CCGR4 *((volatile unsigned int *)0X020C4078)
#define CCM_CCGR5 *((volatile unsigned int *)0X020C407C)
#define CCM_CCGR6 *((volatile unsigned int *)0X020C4080)

void clk_enable(void);
void iimx6ul_clk_init(void);

#endif