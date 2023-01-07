#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

void TaskCount1( void *pvParameters );
void TaskCount2( void *pvParameters );

int count1=0;
int count2=0;

void TaskCount1(void *pvParameters) {
  for( ;; ){
    Serial.print("Count 1: ");
    Serial.println(count1++);
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
  }
}

void TaskCount2(void *pvParameters) {
  for( ;; ){
    Serial.print("Count 2: ");
    Serial.println(count2++);
    vTaskDelay( 2000 / portTICK_PERIOD_MS ); 
  }
}

void setup() {
  Serial.begin(9600);
  xTaskCreate(TaskCount1,"Task1",1000,NULL,1,NULL);
  xTaskCreate(TaskCount2,"Task2 ",1000,NULL,2,NULL); 
  vTaskStartScheduler();
}

void loop(){
  // do nothing
}