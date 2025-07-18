#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "imx6ul.h"
#include "bsp_gpio.h"

/* definition key value*/
typedef enum key_value{
    KEY_NONE = 0,
    KEY0_VALUE = 1,
}key_value_t;

/*function declaration*/
void key_init(void);
int key_getvalue(void);


#endif