/*************************************************************************//**
* @file task_management.cpp
* @brief  Task management example operations
*****************************************************************************/
/*----------------------------------------------------------------------------
  include files
----------------------------------------------------------------------------*/
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include <task_management.h>

/*----------------------------------------------------------------------------
  public functions
----------------------------------------------------------------------------*/
/*************************************************************************//**
* @brief 1 second counter
* @param void *pvParameters pointer value for task parameters
* @return None
*****************************************************************************/
void taskCount1(void *pvParameters) {
  int count1=0;
  char *pcTaskName = (char*) pvParameters;

  for( ;; ){
    Serial.print(pcTaskName);
    Serial.print(" Count: ");
    Serial.println(count1++);

    // vTaskDelay() is number of tick interrupts calling task will remain in Blocked state before tranisitioning to Ready state.
    vTaskDelay(1000 / portTICK_PERIOD_MS); 
  }
}

/*************************************************************************//**
* @brief 2 second counter
* @param void *pvParameters pointer value for task parameters
* @return None
*****************************************************************************/
void taskCount2(void *pvParameters) {
  char *pcTaskName = (char*) pvParameters;
  int count2 = 0;
  const TickType_t xDelay2s = pdMS_TO_TICKS(2000);

  // xLastWakeTime initialised with current tick count, automatically updated within vTaskDelayUntil()
  TickType_t xLastWakeTime = xTaskGetTickCount();

  for( ;; ){
    Serial.print(pcTaskName);
    Serial.print(" Count: ");
    Serial.println(count2++);

    /* vTaskDelayUntil() specifies exact tick count value task should be moved Blocked into Ready state
    used when API function requires fixed execution period as calling task is unblocked is abosolute not relative*/
    vTaskDelayUntil(&xLastWakeTime, xDelay2s);
  }
}

void setup() {
  Serial.begin(9600);

  static const char *pcNameTask1 = "Task 1";
  static const char *pcNameTask2 = "Task 2";

  // create tasks
  xTaskCreate(taskCount1,         // TaskFunction_t pxTaskCode pointer to task function
              "Task1",            // const char* pcName name of task for debug aid
              1000,               // uint16_t usStackDepth tells kernel stack size required
              (void*)pcNameTask1, // void *pvParameters task functions accept parameter type void*
              1,                  // UBaseType_t uxPriority task priority (0 to configMAX_PRIORITIES-1)
              NULL                // TaskHandle_t *pxCreatedTask task handle to reference the task in API calls
              );
  xTaskCreate(taskCount2,"Task2 ",1000,(void*)pcNameTask2,2,NULL); 

  // Starts the RTOS scheduler, after calling the RTOS kernel has control over which tasks are executed and when
  // only returns when insufficent RTOS heap available
  vTaskStartScheduler();
}

void loop(){
  // do nothing
}