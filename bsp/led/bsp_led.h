#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "imx6ul.h"
#include "bsp_gpio.h"

#define LED0 GPIO1,3

void led_init(void);
void led_on(void);
void led_off(void);
void led_switch(GPIO_Type *base, int pin);

#endif