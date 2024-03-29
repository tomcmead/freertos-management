/*************************************************************************//**
* @file software_timer_management.cpp
* @brief  Software timer management example operations
*****************************************************************************/
/*----------------------------------------------------------------------------
  include files
----------------------------------------------------------------------------*/
#include <software_timer_management.h>
#include <timers.h>

/*----------------------------------------------------------------------------
  public functions
----------------------------------------------------------------------------*/
/*************************************************************************//**
* @brief Timer callback resets OneShot timer, doubles period of AutoReload
*        timer after 4 callbacks, stops time after 10 callbacks
* @param TimerHandle_t xTimer timer handler
* @return None
*****************************************************************************/
void prvTimerCallback(TimerHandle_t xTimer){
    static uint32_t ulExecutionCount = 0;
    ulExecutionCount++;

    uint32_t ulTimerID = (uint32_t) pvTimerGetTimerID(xTimer);
    const char *pcTimerName = pcTimerGetName(xTimer);

    Serial.print(pcTimerName);
    Serial.print(" ");
    Serial.print(ulTimerID);
    Serial.print(" callback: ");
    Serial.println(ulExecutionCount);

    if(pcTimerName == "OneShot"){
        xTimerReset(xTimer,                 //TimerHandle_t xTimer,
                    ONE_SHOT_TIMER_PERIOD); // TickType_t xBlockTime
        Serial.print(pcTimerName);
        Serial.println(" timer reset");
    }
    if(ulExecutionCount == 4){
        xTimerChangePeriod(xTimer,                     // TimerHandle_t xTimer
                           AUTO_RELOAD_TIMER_PERIOD*2, // TickType_t xNewTimerPeriodInTicks
                           0);                         // TickType_t xTicksToWait
    }
    else if(ulExecutionCount > 10){
        Serial.print(pcTimerName);
        Serial.println(" timer stopped");
        xTimerStop(xTimer, 0);
    }
}

/*************************************************************************//**
* @brief Create and start a AutoLoad and OneShot timer
* @param void
* @return int
*****************************************************************************/
int main( void ){
    Serial.begin(9600);

    TimerHandle_t xAutoReloadTimer, xOneShotTimer;
    BaseType_t xTimer1Started, xTimer2Started;

    // One shot timer - does not automatically reset itself, so will only expire
    // and therefore execute its callback once unless it is manually restarted
    xOneShotTimer = xTimerCreate("OneShot",             //const char * const pcTimerName
                                 ONE_SHOT_TIMER_PERIOD, // TickType_t xTimerPeriodInTicks
                                 pdFALSE,               // UBaseType_t uxAutoReload (one shot)
                                 0,                     // void * pvTimerID (no timer id)
                                 prvTimerCallback);     // TimerCallbackFunction_t pxCallbackFunction

    // Auto reload timer - resets itself each time it expires, causing the timer to expire
    // and therefore execute its callback periodically
    xAutoReloadTimer = xTimerCreate("AutoReload",             //const char * const pcTimerName
                                    AUTO_RELOAD_TIMER_PERIOD, // TickType_t xTimerPeriodInTicks
                                    pdTRUE,                   // UBaseType_t uxAutoReload (auto reload)
                                    0,                        // void * pvTimerID (no timer id)
                                    prvTimerCallback);        // TimerCallbackFunction_t pxCallbackFunction

    if((xOneShotTimer != NULL) && (xAutoReloadTimer != NULL)){
        xTimer1Started = xTimerStart(xOneShotTimer, // TimerHandle_t xTimer
                                     0);            // TickType_t xTicksToWait (no block time)
        xTimer2Started = xTimerStart(xAutoReloadTimer, 0);

        // xTimerStart() uses the timer command queue - fails if queue full
        // Timer service task not created until the scheduler starts, so queue not processed until it starts
        if((xTimer1Started == pdPASS) && (xTimer2Started == pdPASS)){
            vTaskStartScheduler();
        }
    }

    for(;;);
}

void loop(){
  // do nothing
}
