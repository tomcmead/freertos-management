/*************************************************************************//**
* @file software_timer_management.h
* @brief Function prototypes for software_timer_management operation
*****************************************************************************/
#ifndef _SOFTWARE_TIMER_MANAGEMENT_H
#define _SOFTWARE_TIMER_MANAGEMENT_H

/*----------------------------------------------------------------------------
  nested include files
----------------------------------------------------------------------------*/
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

/*----------------------------------------------------------------------------
  manifest constants
----------------------------------------------------------------------------*/
#define ONE_SHOT_TIMER_PERIOD pdMS_TO_TICKS(5000)
#define AUTO_RELOAD_TIMER_PERIOD pdMS_TO_TICKS(1000)

/*----------------------------------------------------------------------------
  prototypes
----------------------------------------------------------------------------*/
void prvTimerCallback(TimerHandle_t xTimer);

#endif // _SOFTWARE_TIMER_MANAGEMENT_H