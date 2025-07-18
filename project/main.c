#include "imx6ul.h"
// BSP驱动
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_gpio.h"
#include "bsp_int.h"
#include "bsp_exti.h"
#include "bsp_uart.h"
/*
 * @description : main 函数
 * @param : 无
 * @return : 无
 */
int main(void)
{
    int_init(); //初始化中断向量表，最先调用
    // iimx6ul_clk_init(); /* 初始化系统时钟 */
    clk_enable(); /* 使能所有的时钟 */
    // led_init();  //qemu不存在这个设备
    // beep_init(); //qemu不存在这个设备
    // exti_init();
    // gpio_softint(GPIO1, 8);
    uart_init();

    while (1)
    {
        puts("123!");
        delay(1000);
    }
    return 0;
}
