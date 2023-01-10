/*************************************************************************//**
* @file queue_management.h
* @brief Function prototypes for queue_management operation
*****************************************************************************/
#ifndef _QUEUE_MANAGEMENT_H
#define _QUEUE_MANAGEMENT_H

/*----------------------------------------------------------------------------
  nested include files
----------------------------------------------------------------------------*/
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include <queue.h>

/*----------------------------------------------------------------------------
  prototypes
----------------------------------------------------------------------------*/
void vSenderTask(void *pvParameters);
void vReceiverTask(void *pvParameters);

#endif // _QUEUE_MANAGEMENT_H