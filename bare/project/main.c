#include "imx6ul.h"
// BSP驱动
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_gpio.h"
#include "bsp_exti.h"
#include "bsp_epittimer.h"

/*
 * @description : main 函数
 * @param : 无
 * @return : 无
 */
int main(void)
{
    
    int_init(); //初始化中断向量表，最先调用
    iimx6ul_clk_init(); /* 初始化系统时钟 */
    clk_enable(); /* 使能所有的时钟 */
    delay_init();/*初始化GPT定时器*/
    uart_init();
    // epit1_init(3, 66000000); //没反应，可能是没有该设备
    // gpt1_init(3000000);



  

    while (1)
    {
       
        delayms(1000);
        printf("10");
    }
    return 0;
}


