/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

SemaphoreHandle_t xSemaphore;

static void blink( void *pvParameters )
{
    while(1)
    {
        xSemaphoreTake(xSemaphore, portMAX_DELAY);
        P1OUT ^= BIT0;
    }
}

static void sem( void *pvParameters )
{
    while(1)
    {
         xSemaphoreGive(xSemaphore);
         vTaskDelay(500);
    }
}

int main( void )
{
    xSemaphore = xSemaphoreCreateBinary();

    /* Configure system. */
    WDTCTL = WDTPW | WDTHOLD;
    P1DIR |= BIT0;

    /* Create the Task */
    xTaskCreate( blink,                             /* The function that implements the task. */
                "Blink",                            /* The text name assigned to the task - for debug only as it is not used by the kernel. */
                configMINIMAL_STACK_SIZE,           /* The size of the stack to allocate to the task. */
                NULL,                               /* The parameter passed to the task - not used in this case. */
                2,                                  /* The priority assigned to the task. */
                NULL );                             /* The task handle is not required, so NULL is passed. */

    xTaskCreate( sem,                               /* The function that implements the task. */
                "Sem",                              /* The text name assigned to the task - for debug only as it is not used by the kernel. */
                configMINIMAL_STACK_SIZE,           /* The size of the stack to allocate to the task. */
                NULL,                               /* The parameter passed to the task - not used in this case. */
                2,                                  /* The priority assigned to the task. */
                NULL );                             /* The task handle is not required, so NULL is passed. */

    // xSemaphoreGive(xSemaphore);

    /* Start RTOS */
    vTaskStartScheduler();

}

