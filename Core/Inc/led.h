/**
  ******************************************************************************
  * @file    led.h
  * @brief   This file contains all the function prototypes for
  *          the led.c file
  ******************************************************************************
  */

 //DEFINES

 #ifndef LED_H
 #define LED_H

//Includes
#include <stdint.h>

#define LED_PORT    LD2_GPIO_Port
#define LED_PIN     LD2_Pin

#define SYSTICK_FREQ 1000

int32_t led_set_blinkspeed(uint32_t blinkspeed);

uint32_t led_read_blinkspeed(void);

int32_t led_enable(void);

int32_t led_disable(void);

int32_t led_update(void);









 #endif //LED_H