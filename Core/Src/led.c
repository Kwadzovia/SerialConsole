#include "led.h"
#include "main.h"

static uint32_t ui32LedEnabled = 0;
static uint32_t ui32BlinkLength = 0;
static uint32_t ui32BlinkTick = 0;
static uint32_t userBlinkspeed = 0;

int32_t led_set_blinkspeed(uint32_t blinkspeed)
{
    userBlinkspeed = blinkspeed;
    ui32BlinkLength = (SYSTICK_FREQ)/(2*blinkspeed);
}

int32_t led_enable(void)
{
    ui32LedEnabled = ENABLE;
    HAL_GPIO_WritePin(LED_PORT,LED_PIN,GPIO_PIN_SET);
    ui32BlinkTick = HAL_GetTick();
}

int32_t led_disable(void)
{
    ui32LedEnabled = DISABLE;
    HAL_GPIO_WritePin(LED_PORT,LED_PIN,GPIO_PIN_RESET);
}

int32_t led_update(void)
{
    
    if(!ui32LedEnabled || !ui32BlinkLength)
    {
        return SUCCESS;
    }
    
    if(HAL_GetTick() < (ui32BlinkTick))
    {
        ui32BlinkTick = HAL_GetTick();
    }

    if(HAL_GetTick() > (ui32BlinkTick+ui32BlinkLength))
    {
        HAL_GPIO_TogglePin(LED_PORT,LED_PIN);
        ui32BlinkTick = HAL_GetTick();
    }

    return SUCCESS;
}

uint32_t led_read_blinkspeed(void)
{
    return (SYSTICK_FREQ)/(2*ui32BlinkLength);
}