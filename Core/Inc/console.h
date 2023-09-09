/**
  ******************************************************************************
  * @file    led.h
  * @brief   This file contains all the function prototypes for
  *          the led.c file
  ******************************************************************************
  */

#ifndef CONSOLE_H
#define CONSOLE_H 

//Includes
#include "stdint.h"

//DEFINES
#define ASCII_BACKSPACE    8 
#define ASCII_ENTER       13
#define MAX_READBACK_CHARS 10
// #define CONSOLE_DEBUG


//ENUMS
enum eConsoleState
{
  E_CONSOLE_READ_OPTION = 0,
  E_CONSOLE_SET_LED_BLINKSPEED,
};

enum eUserOptions
{
  E_USER_OPTION_ENABLE_LED = 1,
  E_USER_OPTION_DISABLE_LED,
  E_USER_OPTION_SET_LED_SPEED,
  E_USER_OPTION_READ_LED_SPEED,
};

//GLOBALS
extern uint8_t g_ui8NewDataFlag;

//Functions
void consoleInit(void);
void consoleUpdate(void);
static void consoleCheckInput(void);
static void consoleProcessBuffer(void);
static void consoleHomePrint(void);
static void consoleReturn(void);

#endif //CONSOLE_H
