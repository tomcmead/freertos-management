/*************************************************************************//**
* @file queue_management.cpp
* @brief  Queue management example operations
*****************************************************************************/
/*----------------------------------------------------------------------------
  include files
----------------------------------------------------------------------------*/
#include <queue_management.h>

/*----------------------------------------------------------------------------
  global variables
----------------------------------------------------------------------------*/
static QueueHandle_t xQueue;

/*----------------------------------------------------------------------------
  public functions
----------------------------------------------------------------------------*/
/*************************************************************************//**
* @brief Send data to queue task
* @param void *pvParameters pointer value for task parameters
* @return None
*****************************************************************************/
void vSenderTask(void *pvParameters){
  BaseType_t xStatus;
  int32_t lValueToSend = (int32_t) pvParameters;  // cast parameter to required type

  for( ;; ) {
    // Send value to queue
    xStatus = xQueueSendToBack(xQueue,        // QueueHandle_t xQueue - queue data is being sent
                               &lValueToSend, // const void *pvItemToQueue - address of data to be sent 
                               0);            // TickType_t xTicksToWait - time task kept in Blocked state to wait for
                                              // space to become available on the queue should the queue be full

    // Full queue generates error
    if( xStatus != pdPASS )
      Serial.println( "Could not send to the queue\n" );
    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}
/*************************************************************************//**
* @brief Receive data from queue task
* @param void *pvParameters pointer value for task parameters
* @return None
*****************************************************************************/
void vReceiverTask( void *pvParameters ){
  int32_t lReceivedValue; 
  BaseType_t xStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

  for( ;; ){
    // Check queue is empty
    if(uxQueueMessagesWaiting(xQueue) != 0)
      Serial.println( "Queue should have been empty\n" );

    // Receive data from the queue.
    xStatus = xQueueReceive(xQueue,          //QueueHandle_t xQueue
                            &lReceivedValue, //void *pvBuffer - buffer for received data
                            xTicksToWait);   // TickType_t xTicksToWait - - time task kept in Blocked state to wait for
                                             // data to become available on the queue should the queue be empty
    if( xStatus == pdPASS ){
      Serial.print("Received = ");   // Data successfully received from queue
      Serial.println(lReceivedValue);
    }
    else
      Serial.println("Could not receive from the queue\n"); // Data not received from queue even after waiting 100ms
  } 
}


void setup() {
  Serial.begin(9600);
  
  int32_t iData = 10;

  // create tasks
  xTaskCreate(vSenderTask, "SenderTask", 1000, (void*)iData, 1, NULL); 
  xTaskCreate(vReceiverTask, "ReceiverTask", 1000, NULL, 1, NULL); 

  // Starts the RTOS scheduler
  vTaskStartScheduler();
}

void loop(){
  // do nothing
}