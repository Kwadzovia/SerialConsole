#include "main.h"
#include "console.h"
#include "usart.h"
#include <string.h>
#include "led.h"

uint8_t g_ui8NewDataFlag = 0;

static uint8_t ui8RxChar = 0;
static uint8_t ui8RxIndex = 0;
static uint8_t aui8RxBuffer[0x100] = {0};
static eConsoleState = E_CONSOLE_READ_OPTION;

void consoleInit(void)
{
  led_set_blinkspeed(1);
  consoleHomePrint();
  HAL_UART_Receive_IT(&huart2,&ui8RxChar,1);
}

void consoleUpdate(void)
{
    consoleCheckInput();
}

static void consoleProcessBuffer(void)
{
  uint32_t ui32UserInput;
  uint32_t ui32InvalidInput = 0;
  uint32_t clearIndex = 0;

  char acCharReadback[MAX_READBACK_CHARS];

  switch(eConsoleState)
  {
    case E_CONSOLE_READ_OPTION:

      ui32UserInput = (uint32_t)atoi(aui8RxBuffer);
        if(ui32UserInput == E_USER_OPTION_SET_LED_SPEED)
        {
          eConsoleState = E_CONSOLE_SET_LED_BLINKSPEED;
          HAL_UART_Transmit(&huart2, "\r\nEnter Blinkspeed...\r\n", 24, HAL_UART_STATE_TIMEOUT);
        }
        else if(ui32UserInput == E_USER_OPTION_READ_LED_SPEED)
        {   
          for(clearIndex = 0;clearIndex < MAX_READBACK_CHARS; clearIndex++)
          {
            acCharReadback[clearIndex] = '\0';
          }
          ui32UserInput = led_read_blinkspeed();
          snprintf(acCharReadback, MAX_READBACK_CHARS,"%d",ui32UserInput);
          HAL_UART_Transmit(&huart2, "\r\nLED BLINKSPEED:\t", 19, HAL_UART_STATE_TIMEOUT);
          HAL_UART_Transmit(&huart2, acCharReadback, MAX_READBACK_CHARS, HAL_UART_STATE_TIMEOUT);
          HAL_UART_Transmit(&huart2, "\r\n", 3, HAL_UART_STATE_TIMEOUT);
          consoleReturn();
        }
        else if(ui32UserInput == E_USER_OPTION_ENABLE_LED)
        {
          led_enable();
          consoleReturn();
        }
        else if(ui32UserInput == E_USER_OPTION_DISABLE_LED)
        {
          led_disable();
          consoleReturn();
        }
        else
        {
          ui32InvalidInput = 1;
        }
      break;

    case E_CONSOLE_SET_LED_BLINKSPEED:

      ui32UserInput = (uint32_t)atoi(aui8RxBuffer);
      ui32InvalidInput = (ui32UserInput == 0) ? 1 : 0;

      if(!ui32InvalidInput)
      {
        led_set_blinkspeed(ui32UserInput);
      }
      consoleReturn();
      break;

      
  }

  if(ui32InvalidInput)
  {
    HAL_UART_Transmit(&huart2, "\nError: \"", 10, HAL_UART_STATE_TIMEOUT);
    HAL_UART_Transmit(&huart2, aui8RxBuffer, ui8RxIndex, HAL_UART_STATE_TIMEOUT);
    HAL_UART_Transmit(&huart2, "\" is not a valid input.\r\n", 26, HAL_UART_STATE_TIMEOUT);
    consoleReturn();
  }
  
}

static void consoleHomePrint(void)
{
    HAL_UART_Transmit(&huart2, "\r\nEnter an option...\r\n1:\tEnable LED\r\n2:\tDisable LED\r\n3:\tSet LED Blinkspeed\r\n4:\tRead LED Blinkspeed\r\n\n", 102, HAL_UART_STATE_TIMEOUT);
}

static void consoleReturn(void)
{
    consoleHomePrint();
    eConsoleState = E_CONSOLE_READ_OPTION;
}
static void consoleCheckInput(void)
{
    if(g_ui8NewDataFlag)
    {
      if(ui8RxChar == ASCII_BACKSPACE)
      {
        HAL_UART_Transmit(&huart2, "\b \b", 3, HAL_UART_STATE_TIMEOUT);
        if(ui8RxIndex > 0)
        {
          ui8RxIndex--;
        }
      }
      else if(ui8RxChar == ASCII_ENTER)
      {
        HAL_UART_Transmit(&huart2, "\r\n", 2, HAL_UART_STATE_TIMEOUT);
#ifdef CONSOLE_DEBUG
        HAL_UART_Transmit(&huart2, "YOU TYPED:", 11, HAL_UART_STATE_TIMEOUT);
        HAL_UART_Transmit(&huart2, aui8RxBuffer, ui8RxIndex, HAL_UART_STATE_TIMEOUT);
#endif
        aui8RxBuffer[ui8RxIndex] = '\0';
        consoleProcessBuffer();

        aui8RxBuffer[0] = ' ';
        ui8RxIndex = 0;
      }
      else
      {
        aui8RxBuffer[ui8RxIndex] = ui8RxChar;
        HAL_UART_Transmit(&huart2, &aui8RxBuffer[ui8RxIndex], 1, HAL_UART_STATE_TIMEOUT);
        ui8RxIndex++; //NOTE Add Bound checking
      }

      // HAL_UART_Transmit(&huart2, '\n', 1, HAL_UART_STATE_TIMEOUT);
      g_ui8NewDataFlag = 0;
      HAL_UART_Receive_IT(&huart2,&ui8RxChar,1);
    }
}