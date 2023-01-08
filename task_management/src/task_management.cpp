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

  for( ;; ){
    Serial.print("Count 1: ");
    Serial.println(count1++);
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
  }
}

/*************************************************************************//**
* @brief 2 second counter
* @param void *pvParameters pointer value for task parameters
* @return None
*****************************************************************************/
void taskCount2(void *pvParameters) {
  int count2=0;

  for( ;; ){
    Serial.print("Count 2: ");
    Serial.println(count2++);
    vTaskDelay( 2000 / portTICK_PERIOD_MS ); 
  }
}

void setup() {
  Serial.begin(9600);
  xTaskCreate(taskCount1,"Task1",1000,NULL,1,NULL);
  xTaskCreate(taskCount2,"Task2 ",1000,NULL,2,NULL); 
  vTaskStartScheduler();
}

void loop(){
  // do nothing
}