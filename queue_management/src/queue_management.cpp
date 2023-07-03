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
// Queue created to hold a max of 3 structures of type Data_t
static QueueHandle_t xQueue = xQueueCreate(3,               // UBaseType_t uxQueueLength
                                           sizeof(Data_t));  // UBaseType_t uxItemSize

// Declare two variables of type Data_t that will be passed on the queue
static const Data_t xStructsToSend[2] = {
    {100, eSender1}, /* Used by Sender1. */
    {200, eSender2}  /* Used by Sender2. */
};

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
    const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

    for( ;; ) {
    // Send value to queue equivalent to xQueueSend()
    xStatus = xQueueSendToBack(xQueue,        // QueueHandle_t xQueue - queue data is being sent
                               pvParameters,  // const void *pvItemToQueue - address of data to be sent 
                               xTicksToWait); // TickType_t xTicksToWait - time task kept in Blocked state to wait for
                                              // space to become available on the queue should the queue be full

        // Full queue generates error
        if(xStatus != pdPASS)  // returns either pdPASS or errQUEUE_FULL
        Serial.println( "Could not send to the queue\n" );

    // if(xQueueSendToFront(xQueue, &lValueToSend, 0) != pdPASS) 
    //   Serial.println( "Could not send to the queue\n" );

      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/*************************************************************************//**
* @brief Receive data from queue task
* @param void *pvParameters pointer value for task parameters
* @return None
*****************************************************************************/
void vReceiverTask( void *pvParameters ){
    Data_t xReceivedStructure; 
    BaseType_t xStatus;
    const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

    for( ;; ){
        // Check queue is empty
        if(uxQueueMessagesWaiting(xQueue) != 0)
            Serial.println( "Queue should have been empty\n" );

        // Receive data from the queue.
        xStatus = xQueueReceive(xQueue,          //QueueHandle_t xQueue
                                &xReceivedStructure, //void *pvBuffer - buffer for received data
                                xTicksToWait);   // TickType_t xTicksToWait - - time task kept in Blocked state to wait for
                                              // data to become available on the queue should the queue be empty
        if(xStatus == pdPASS){  // returns pdPASS or errQUEUE_EMPTY
            if(xReceivedStructure.eDataSource == eSender1){
                Serial.print("From Sender 1 = "); 
                 Serial.println(xReceivedStructure.ucValue);
            } else if(xReceivedStructure.eDataSource == eSender2){
                Serial.print("From Sender 2 = "); 
                Serial.println(xReceivedStructure.ucValue);
            }
        }
        else
            Serial.println("Could not receive from the queue\n"); // Data not received from queue even after waiting 100ms
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(9600);

    if(xQueue != NULL){

        // create write queue tasks, send xStructsToSend[0] and xStructsToSend[1] data
        xTaskCreate(vSenderTask, "Sender1", 1000, (void*) &(xStructsToSend[0]), 2, NULL);
        xTaskCreate(vSenderTask, "Sender2", 1000, (void*) &(xStructsToSend[1]), 2, NULL);

        // create read queue task with priority lower than sender tasks
        xTaskCreate(vReceiverTask, "Receiver", 1000, NULL, 1, NULL);

        // Starts the RTOS scheduler
        vTaskStartScheduler();
    } else
        Serial.println("Could not create queue\n");
}

void loop(){
  // do nothing
}