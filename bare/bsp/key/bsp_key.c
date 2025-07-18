#include "bsp_key.h"

void key_init(void){
    /* mux and config */
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);
    /* initialize pin input*/
    gpio_pin_config_t key_config;
    key_config.direction = kGPIO_DigitalInput;
    key_config.interruptMode = 0;
    gpio_init(GPIO1,18, &key_config);
}

/* 输入寄存器0表示按下，1表示升起*/
int key_getvalue(void){
    key_value_t ret = KEY_NONE;
    static uint8_t relese = 1;
    if(relese && !gpio_pinread(GPIO1, 18)){
        delay(10);
        if(!gpio_pinread(GPIO1, 18)){
            relese = 0;
            ret = KEY0_VALUE;
        }
    }else if(gpio_pinread(GPIO1, 18)){
        relese = 0;
        ret = KEY_NONE;
    }
    return ret;
}

// int key_getvalue(void)
// {
// int ret = 0;
// static unsigned char release = 1; /* 按键松开 */

// if((release==1)&&(gpio_pinread(GPIO1, 18) == 0)) /* KEY0 按下 */
// { 
// delay(10); /* 延时消抖 */
// release = 0; /* 标记按键按下 */
// if(gpio_pinread(GPIO1, 18) == 0)
// ret = KEY0_VALUE;
// }
// else if(gpio_pinread(GPIO1, 18) == 1) /* KEY0 未按下 */
// {
// ret = 0;
// release = 1; /* 标记按键释放 */
// }

// return ret;
// }
