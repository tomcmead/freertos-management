/*************************************************************************//**
* @file task_management.cpp
* @brief  Task management example operations
*****************************************************************************/
/*----------------------------------------------------------------------------
  include files
----------------------------------------------------------------------------*/
#include <task_management.h>

/*----------------------------------------------------------------------------
  global variables
----------------------------------------------------------------------------*/
TaskHandle_t xTask1Handle = NULL;

/*----------------------------------------------------------------------------
  public functions
----------------------------------------------------------------------------*/
/*************************************************************************//**
* @brief 1 second counter
* @param void *pvParameters pointer value for task parameters
* @return None
*****************************************************************************/
void vTask1Count(void *pvParameters) {
  uint16_t uCount1=0;
  char *pcTaskName = (char*) pvParameters; // get char parameters
  UBaseType_t uxPriority = uxTaskPriorityGet(NULL); /* query the priority at which this task is running
                                                      passing in NULL means return the calling task’s priority*/

  for( ;; ){
    Serial.print(pcTaskName);
    Serial.print(" Count: ");
    Serial.println(uCount1++);

    // vTaskDelay() is number of tick interrupts calling task will remain in Blocked state before tranisitioning to Ready state
    vTaskDelay(1000 / portTICK_PERIOD_MS); 

    // Set task1 
    vTaskPrioritySet(NULL, (uxPriority - 2));
  }
}

/*************************************************************************//**
* @brief 2 second counter
* @param void *pvParameters pointer value for task parameters
* @return None
*****************************************************************************/
void vTask2Count(void *pvParameters) {
  char *pcTaskName = (char*) pvParameters; // get char parameters
  uint16_t uCount2 = 0;
  const TickType_t xDelay2s = pdMS_TO_TICKS(2000); // ms to ticks functions
  UBaseType_t uxPriority = uxTaskPriorityGet(NULL); /* query the priority at which this task is running
                                                       passing in NULL means return the calling task’s priority*/

  // xLastWakeTime initialised with current tick count, automatically updated within vTaskDelayUntil()
  TickType_t xLastWakeTime = xTaskGetTickCount();

  for( ;; ){
    Serial.print(pcTaskName);
    Serial.print(" Count: ");
    Serial.println(uCount2++);

    /* vTaskDelayUntil() specifies exact tick count value task should be moved Blocked into Ready state
    used when API function requires fixed execution period as calling task is unblocked is abosolute not relative*/
    vTaskDelayUntil(&xLastWakeTime, xDelay2s);

    // Increase Task1 priority
    vTaskPrioritySet(xTask1Handle, (uxPriority + 1));
  }
}


/*************************************************************************//**
* @brief Idle hook functions MUST be called vApplicationIdleHook(), take no 
*        parameters, return void. Example counter.
* @param void 
* @return None
*****************************************************************************/
void vApplicationIdleHook(void)
{
  static volatile uint32_t ulIdleCycleCount = 0UL;  // Declare a variable that will be incremented by the hook function
  if(!(ulIdleCycleCount % 50000)){
    Serial.print("Idle Hook Count: ");
    Serial.println(ulIdleCycleCount);
  }
  ulIdleCycleCount++;
}

void setup() {
  Serial.begin(9600);

  static const char *pcNameTask1 = "Task 1";
  static const char *pcNameTask2 = "Task 2";

  // create tasks
  xTaskCreate(vTask1Count,        // TaskFunction_t pxTaskCode pointer to task function
              "Task1",            // const char* pcName name of task for debug aid
              1000,               // uint16_t usStackDepth tells kernel stack size required
              (void*)pcNameTask1, // void *pvParameters task functions accept parameter type void*
              2,                  // UBaseType_t uxPriority task priority (0 to configMAX_PRIORITIES-1)
              &xTask1Handle       // TaskHandle_t *pxCreatedTask task handle to reference the task in API calls
              );
  xTaskCreate(vTask2Count, "Task2", 1000, (void*)pcNameTask2, 1, NULL); 

  // Starts the RTOS scheduler, after calling the RTOS kernel has control over which tasks are executed and when
  // only returns when insufficent RTOS heap available
  vTaskStartScheduler();
}

void loop(){
  // do nothing
}