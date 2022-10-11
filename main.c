/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"
#include "GPIO_cfg.h"

/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )


/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/


/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */


TaskHandle_t Task_1_Handler=NULL;
TaskHandle_t Task_2_Handler=NULL;
TaskHandle_t Task_3_Handler=NULL;
TaskHandle_t Task_4_Handler=NULL;
TaskHandle_t Task_5_Handler=NULL;
TaskHandle_t Task_6_Handler=NULL;
int i=0;
pinState_t Consumer_task1;
pinState_t Consumer_task2;
signed char *Data_recieved;

void Task_1(void *pvParameters){
vTaskSetApplicationTaskTag( NULL, ( void * ) 1 );

for(;;){
Consumer_task1=GPIO_read(PORT_0,PIN2);
vTaskDelay(50);
}
}

void Task_2(void *pvParameters){
vTaskSetApplicationTaskTag( NULL, ( void * ) 2 );

for(;;){
Consumer_task2=GPIO_read(PORT_0,PIN3);
vTaskDelay(50);
}
}
void Task_3(void *pvParameters){
vTaskSetApplicationTaskTag( NULL, ( void * ) 3 );
for(;;){
vSerialPutString("AhmedSayed\n",11);
vTaskDelay(100);
}
}
void Task_4(void *pvParameters){
vTaskSetApplicationTaskTag( NULL, ( void * ) 4 );
for(;;){
xSerialGetChar(Data_recieved);
vTaskDelay(20);
}
}
void Task_5(void *pvParameters){
vTaskSetApplicationTaskTag( NULL, ( void * ) 5 );
for(;;){
i=0;
while(i<12500){i=i+1;}
vTaskDelay(10);
}
}
void Task_6(void *pvParameters){
vTaskSetApplicationTaskTag( NULL, ( void * ) 6 );
for(;;){
i=0;
while(i<30000){i=i+1;}
vTaskDelay(100);
}
}
int main( void )
	{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();

	
    /* Create Tasks here */
	
	#if( configUSE_EDF_SCHEDULER == 1 )
	xTaskCreatePeriodic(Task_1,"Button_1_Monitor",100,(void*)0,1,&Task_1_Handler,50);
	xTaskCreatePeriodic(Task_2,"Button_2_Monitor",100,(void*)0,1,&Task_2_Handler,50);
	xTaskCreatePeriodic(Task_3,"Periodic_Transmitter",100,(void*)0,1,&Task_3_Handler,100);
	xTaskCreatePeriodic(Task_4,"Uart_Receiver",100,(void*)0,1,&Task_4_Handler,20);
	xTaskCreatePeriodic(Task_5,"Load_1_Simulation",100,(void*)0,1,&Task_5_Handler,10);
	xTaskCreatePeriodic(Task_6,"Load_2_Simulation",100,(void*)0,1,&Task_6_Handler,100);
	#else
   	xTaskCreate(Task_1,"Task1",100,(void*)0,1,&Task_1_Handler);
		xTaskCreate(Task_2,"Task2",100,(void*)0,1,&Task_2_Handler);
	#endif
	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();
	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

/* Function to reset timer 1 */
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();
	
	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
 
}
/*-----------------------------------------------------------*/
/*void Button_Task (void *pvParameters ){
		for(;;){
		GPIO_write( PORT_0,PIN2, PIN_IS_HIGH);
		vTaskDelay(500);
		GPIO_write( PORT_0,PIN2, PIN_IS_LOW);
		vTaskDelay(500);
		}
}
*/

void vApplicationTickHook(void) {
GPIO_write( PORT_0,PIN0, PIN_IS_HIGH);
GPIO_write( PORT_0,PIN0, PIN_IS_LOW);
}
void vApplicationIdleHook(void) {
vTaskSetApplicationTaskTag( NULL, ( void * ) 0 );

}