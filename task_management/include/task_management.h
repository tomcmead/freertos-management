/*************************************************************************//**
* @file task_management.h
* @brief Function prototypes for task_management operation
*****************************************************************************/
#ifndef _TASK_MANAGEMENT_H
#define _TASK_MANAGEMENT_H

/*----------------------------------------------------------------------------
  nested include files
----------------------------------------------------------------------------*/
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

/*----------------------------------------------------------------------------
  prototypes
----------------------------------------------------------------------------*/
void vTask1Count( void *pvParameters );
void vTask2Count( void *pvParameters );

#endif // _TASK_MANAGEMENT_H