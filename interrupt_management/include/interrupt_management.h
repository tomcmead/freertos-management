/*************************************************************************//**
* @file interrupt_management.h
* @brief Function prototypes for interrupt_management operation
*****************************************************************************/
#ifndef _INTERRUPT_MANAGEMENT_H
#define _INTERRUPT_MANAGEMENT_H

/*----------------------------------------------------------------------------
  nested include files
----------------------------------------------------------------------------*/
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

/*----------------------------------------------------------------------------
  manifest constants
----------------------------------------------------------------------------*/
#define INTERRUPT_0_PIN 21

/*----------------------------------------------------------------------------
  prototypes
----------------------------------------------------------------------------*/
static void vPeriodicTask(void *pvParameters);
static void vHandlerTask(void *pvParameters);
static void ulInterruptHandler(void);

/*----------------------------------------------------------------------------
  type definitions
----------------------------------------------------------------------------*/


#endif // _INTERRUPT_MANAGEMENT_H