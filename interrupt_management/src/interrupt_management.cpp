/*************************************************************************//**
* @file interrupt_management.cpp
* @brief  Interrupt management example operations
*         lowest priority interrupt will interrupt highest priority task
*****************************************************************************/
/*----------------------------------------------------------------------------
  include files
----------------------------------------------------------------------------*/
#include <interrupt_management.h>
#include <semphr.h>

/*----------------------------------------------------------------------------
  global variable
----------------------------------------------------------------------------*/
SemaphoreHandle_t xBinarySemaphore;

/*----------------------------------------------------------------------------
  public functions
----------------------------------------------------------------------------*/
/*************************************************************************//**
* @brief Software interrupt triggered every 0.5S
* @param void *pvParameters
* @return None
*****************************************************************************/
void vPeriodicTask(void *pvParameters){
    const TickType_t xDelay500ms = pdMS_TO_TICKS(500UL);

    for( ;; ){
        vTaskDelay( xDelay500ms ); // Block time until software interrupt generated

        Serial.println("Periodic Task: generate software interrupt");

        // Invert value on INT0 pin to trigger a software interrupt
        digitalWrite(INTERRUPT_0_PIN, ~digitalRead(INTERRUPT_0_PIN));
    }
}

/*************************************************************************//**
* @brief ISR processing deferred to this HandlerTask
* @param None
* @return None
*****************************************************************************/
void vHandlerTask(void *pvParameters){
    for( ;; ){
        xSemaphoreTake(xBinarySemaphore, portMAX_DELAY); // Task blocks until semaphore obtained
        Serial.println("Handler Task: processing software interrupt\n");
    }
}

/*************************************************************************//**
* @brief Interrupt handler release binary semaphore
* @param None
* @return None
*****************************************************************************/
void ulInterruptHandler(void){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE; // set true inside interrupt safe function
    xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(); // Request context switch
}

int main(void){
    Serial.begin(9600);
    pinMode(INTERRUPT_0_PIN, INPUT_PULLUP);

    xBinarySemaphore = xSemaphoreCreateBinary();

    if(xBinarySemaphore != NULL){
        // Handler task for which interrupt processing is deferred, high priority
        xTaskCreate(vHandlerTask, "Handler", 1000, NULL, 3, NULL);

        // Periodically generate software interrupt task, lower priority
        xTaskCreate(vPeriodicTask, "Periodic", 1000, NULL, 1, NULL);

        // Set interrupt INT0 with ulInterruptHandler function on pin change
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_0_PIN), ulInterruptHandler, CHANGE);

        vTaskStartScheduler();
    }
    for( ;; );
}

void loop(){
  // do nothing
}