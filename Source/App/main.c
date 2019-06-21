#include <stdio.h>
#include <stdlib.h>

#include "stm32f7xx_hal.h"
#include "bsp.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//#include "ethernetif.h"
//#include "lwip/netif.h"
//#include "lwip/tcpip.h"
//#include "lwip/sockets.h"

#define task_PRIORITY_LED_BLINK 5
#define task_PRIORITY_GUI       6

static void prvStartThread(void * pvParameters);

extern void task_GUI( void *parameter );
extern void task_LEDBlink( void *parameter );
extern void task_SocketSeriver( void *parameter );




void main(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  debug_printf("start...\r\n");

  bsp_init();

  xTaskCreate( prvStartThread,                  /* The function that implements the task. */
               "Start Thread",                  /* The text name assigned to the task - for debug only as it is not used by the kernel. */
               configMINIMAL_STACK_SIZE * 5,    /* The size of the stack to allocate to the task. */
               NULL,                            /* The parameter passed to the task - not used in this case. */
               0,                               /* The priority assigned to the task. */
               NULL );                          /* The task handle is not required, so NULL is passed. */

  vTaskStartScheduler();/* Start the tasks and timer running. */

  for( ;; )
  {
  }
}


static void prvStartThread(void * pvParameters)
{
  xTaskCreate( task_LEDBlink,
               "Blink LED",
               configMINIMAL_STACK_SIZE,
               NULL,
               task_PRIORITY_LED_BLINK,
               NULL );

  xTaskCreate( task_GUI,
               "GUI",
               2048,
               NULL,
               task_PRIORITY_GUI,
               NULL );
  
  xTaskCreate( task_SocketSeriver,
               "SocketSeriver",
               2048,
               NULL,
               3,
               NULL );

  for(;;)
  {
  #if (INCLUDE_vTaskDelete == 1)
    vTaskDelete(NULL);
  #endif
  }
}







void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */

	/* Force an assert. */
	configASSERT( ( volatile void * ) NULL );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */

	/* Force an assert. */
	configASSERT( ( volatile void * ) NULL );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
volatile size_t xFreeHeapSpace;

	/* This is just a trivial example of an idle hook.  It is called on each
	cycle of the idle task.  It must *NOT* attempt to block.  In this case the
	idle task just queries the amount of FreeRTOS heap that remains.  See the
	memory management section on the http://www.FreeRTOS.org web site for memory
	management options.  If there is a lot of heap memory free then the
	configTOTAL_HEAP_SIZE value in FreeRTOSConfig.h can be reduced to free up
	RAM. */
	xFreeHeapSpace = xPortGetFreeHeapSize();

	/* Remove compiler warning about xFreeHeapSpace being set but never used. */
	( void ) xFreeHeapSpace;
}
/*-----------------------------------------------------------*/

void vAssertCalled( uint32_t ulLine, const char *pcFile )
{
volatile unsigned long ul = 0;

	( void ) pcFile;
	( void ) ulLine;

	taskENTER_CRITICAL();
	{
		/* Set ul to a non-zero value using the debugger to step out of this
		function. */
		while( ul == 0 )
		{
			__NOP();
		}
	}
	taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/
extern void HAL_IncTick(void);
void vApplicationTickHook( void )
{
  //HAL_IncTick();
}

