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
    (*(uint32_t *)0x00A02004) = 0xFFU;
    (*(uint32_t *)0x00A01000) = 0x1U;
    (*(uint32_t *)0x00A02000) = 0x1U;
    iimx6ul_clk_init(); /* 初始化系统时钟 */
    clk_enable(); /* 使能所有的时钟 */
    delay_init();/*初始化GPT定时器*/
    // led_init();  //qemu不存在这个设备
    // beep_init(); //qemu不存在这个设备
    // exti_init(); //qemu不存在这个设备
    uart_init();
    // epit1_init(3, 66000000); //没反应，可能是没有该设备
    gpt1_init(3000000);
    /* 1. 取 GIC Distributor 基地址（4 KB 对齐） */
         /* 1. GIC Distributor 基地址 */
    uint32_t gicd_base = 0x00A01000;
    /* 2. GIC CPU Interface 基地址 */
    // uint32_t gicc_base = 0x00002000;

    /* 2. 使能中断 0 */
    // volatile uint32_t *isenabler0 = (volatile uint32_t *)(gicd_base + 0x100);
    // *isenabler0 = (1U << 0);
    
    /* 3. 触发 SGI 0（目标 CPU 位图 = 0b0001，即 CPU0） */
    // volatile uint32_t *sgir = (volatile uint32_t *)(gicd_base + 0xF00);
    // *sgir = (1U << 16) | 0U;

    /* 3. 使能 GPT1 的中断号 87（对应 ISENABLER2） */
    volatile uint32_t *isenabler2 = (volatile uint32_t *)(gicd_base + 0x108); // ISENABLER2 控制中断 64~95
    *isenabler2 |= (1U << (87 - 64)); // 87 - 64 = 23，启用第23位


  

    while (1)
    {
       
        delayms(1000);
        printf("10");
    }
    return 0;
}


