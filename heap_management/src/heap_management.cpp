/*************************************************************************//**
* @file heap_management.cpp
* @brief  Heap management example operations
*****************************************************************************/
/*----------------------------------------------------------------------------
  include files
----------------------------------------------------------------------------*/
#include <heap_management.h>

/*----------------------------------------------------------------------------
  public functions
----------------------------------------------------------------------------*/
/*************************************************************************//**
* @brief heap_3 example for dynamic memory allocation
* @param void *pvParameters pointer value for task parameters
* @return None
*****************************************************************************/
void vHeap3Example(void *pvParameters) {
    uint32_t iArrayLength = (uint32_t) pvParameters; // int parameter passed

    // Allocate dynamic single block of memory 
    uint32_t* ptr;
    ptr = (uint32_t*)pvPortMalloc(iArrayLength * sizeof(uint32_t));

    // Get the elements of the array and print
    for (uint32_t i=0; i<iArrayLength; ++i)
        ptr[i] = i + 1;
    for (uint32_t i=0; i<iArrayLength; ++i)
        Serial.print(ptr[i]);
    Serial.println();

    // Dynamically de-allocate the memory
    vPortFree(ptr);

    vTaskDelay(10000 / portTICK_PERIOD_MS);
}

void setup() {
    Serial.begin(9600);
    static const int iArraySize = 10;

    // create tasks
    xTaskCreate(vHeap3Example, "Heap3Task", 1000, (void*)iArraySize, 1, NULL);

    // Starts the RTOS scheduler
    vTaskStartScheduler();
}

void loop(){
    // do nothing
}