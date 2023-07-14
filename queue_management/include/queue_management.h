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

/*----------------------------------------------------------------------------
  type definitions
----------------------------------------------------------------------------*/
// Define an enumerated type used to identify the source of the data 
typedef enum{
  eSender1,
  eSender2,
  eSender3
} DataSource_t;

// Define the structure type that will be passed on the queue
typedef struct{
  uint8_t ucValue;
  DataSource_t eDataSource;
} Data_t;

#endif // _QUEUE_MANAGEMENT_H