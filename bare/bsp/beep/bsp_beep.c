#include "bsp_beep.h"

/*
* @description : 初始化蜂鸣器对应的 IO
* @param : 无
* @return : 无
*/
void beep_init(void)
{
    /* 1、初始化 IO 复用，复用为 GPIO5_IO01 */
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01,0); 

    /* 2、配置 GPIO1_IO03 的 IO 属性 */
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01,0X10B0);

    /* 3、初始化 GPIO,GPIO5_IO01 设置为输出 */
    GPIO5->GDIR |= (1 << 1); 

    /* 4、设置 GPIO5_IO01 输出高电平，关闭蜂鸣器 */
    GPIO5->DR |= (1 << 1); 
}

/*
* @description : 蜂鸣器控制函数，控制蜂鸣器打开还是关闭
* @param - status : 0，关闭蜂鸣器，1 打开蜂鸣器
* @return : 无
*/
void beep_switch(int status)
{ 
    if(status == ON)
    GPIO5->DR &= ~(1 << 1); /* 打开蜂鸣器 */
    else if(status == OFF)
    GPIO5->DR |= (1 << 1); /* 关闭蜂鸣器 */
}