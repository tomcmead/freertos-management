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
static QueueHandle_t xQueue1 = NULL, xQueue2 = NULL, xQueue3 = NULL; // queues to be added to queue set
static QueueSetHandle_t xQueueSet = NULL; // Queue sets allow a task to receive data from more than one queue without
                                          // the task polling each queue in turn to determine which, if any, contains data

// Declare two variables of type Data_t that will be passed on the queue
static const Data_t xStructsToSend[3] = {
    {1, eSender1}, // Used by Sender1
    {2, eSender2}, // Used by Sender2
    {3, eSender3}  // Used by Sender3
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
    Data_t *pxSendStructure = (Data_t *) pvParameters;

    for( ;; ) {
        if(pxSendStructure->eDataSource == eSender1){
           xStatus = xQueueSendToBack(xQueue1,          // QueueHandle_t xQueue - queue data is being sent
                                      &pxSendStructure, // const void *pvItemToQueue - address of data to be sent 
                                      xTicksToWait);    // TickType_t xTicksToWait - time task kept in Blocked state to wait for
                                                     // space to become available on the queue should the queue be full
        }
        else if(pxSendStructure->eDataSource == eSender2){
            xStatus = xQueueSendToFront(xQueue2, &pxSendStructure, xTicksToWait);
        }
        else{
            xStatus = xQueueSend(xQueue3, &pxSendStructure, xTicksToWait);
        }

        // Full queue generates error
        if(xStatus != pdPASS)  // returns either pdPASS or errQUEUE_FULL
            Serial.println( "Could not send to the queue\n" );

        pxSendStructure->ucValue += 1;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/*************************************************************************//**
* @brief Receive data from queue task and queue set task
* @param void *pvParameters pointer value for task parameters
* @return None
*****************************************************************************/
void vReceiverTask( void *pvParameters ){
    Data_t *xReceivedStructure; 
    BaseType_t xStatus;
    const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
    QueueHandle_t xQueueThatContainsData;
    bool bQueueSelect = true;

    for( ;; ){
        if(bQueueSelect){
            /* Read Queue1/Queue2 from Queue Set */
            xQueueThatContainsData = (QueueHandle_t) xQueueSelectFromSet(xQueueSet,      // QueueSetHandle_t xQueueSet
                                                                         portMAX_DELAY); // TickType_t xTicksToWait
        } else{
            /* Read Queue3 */
            xQueueThatContainsData = xQueue3;
        }

        // Receive data from the queue.
        xStatus = xQueueReceive(xQueueThatContainsData, //QueueHandle_t xQueue
                                &xReceivedStructure,    //void *pvBuffer - buffer for received data
                                xTicksToWait);          // TickType_t xTicksToWait - - time task kept in Blocked state to wait for
                                                        // data to become available on the queue should the queue be empty

        if(xStatus == pdPASS){  // returns pdPASS or errQUEUE_EMPTY
            if(xReceivedStructure->eDataSource == eSender1){
                Serial.print("From Sender 1 = ");
                Serial.println(xReceivedStructure->ucValue);
            } else if(xReceivedStructure->eDataSource == eSender2){
                Serial.print("From Sender 2 = ");
                Serial.println(xReceivedStructure->ucValue);
            } else if(xReceivedStructure->eDataSource == eSender3){
                Serial.print("From Sender 3 = ");
                Serial.println(xReceivedStructure->ucValue);
            }
        }
        else
            Serial.println("Could not receive from the queue\n"); // Data not received from queue even after waiting 100ms
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        bQueueSelect = !bQueueSelect;
    }
}

void setup() {
    Serial.begin(9600);

        // Queue created to hold a max of 2 structures of type Data_t
    xQueue1 = xQueueCreate(2,               // UBaseType_t uxQueueLength
                           sizeof(Data_t)); // UBaseType_t uxItemSize
    xQueue2 = xQueueCreate(2, sizeof(Data_t));

    xQueueSet = xQueueCreateSet( 2 * 2 ); // 2 queues added which each can contain 2 items

    // Add the two queues to the set
    xQueueAddToSet(xQueue1,    // QueueSetMemberHandle_t xQueueOrSemaphore
                   xQueueSet); // QueueSetHandle_t xQueueSet
    xQueueAddToSet(xQueue2, xQueueSet);

    // Create write queue tasks, send xStructsToSend data
    xTaskCreate(vSenderTask, "Sender1", 1000, (void*) &(xStructsToSend[0]), 2, NULL);
    xTaskCreate(vSenderTask, "Sender2", 1000, (void*) &(xStructsToSend[1]), 2, NULL);
    xTaskCreate(vSenderTask, "Sender3", 1000, (void*) &(xStructsToSend[2]), 2, NULL);

    // Create read queue task with priority lower than sender tasks
    xTaskCreate(vReceiverTask, "Receiver", 1000, NULL, 1, NULL);

    // Starts the RTOS scheduler
    vTaskStartScheduler();

    for(;;);
}

void loop(){
  // do nothing
}