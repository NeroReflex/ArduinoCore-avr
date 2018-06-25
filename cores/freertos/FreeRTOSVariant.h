/* freeRTOSVariant.h
 *
 * Board variant (hardware) specific definitions for the AVR boards that I use regularly.
 *
 * This file is NOT part of the FreeRTOS distribution.
 *
 */

#ifndef freeRTOSVariant_h
#define freeRTOSVariant_h

#include <avr/io.h>
#include <avr/wdt.h>

#include "task.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef configTICK_RATE_HZ
	#if defined ( portUSE_WDT )
		/* Watchdog period options: 
			WDTO_15MS
			WDTO_30MS
			WDTO_60MS
			WDTO_120MS
			WDTO_250MS
			WDTO_500MS
		*/
		// System Tick - Scheduler timer
		// Use the Watchdog timer, and choose the rate at which scheduler interrupts will occur.
		#define portUSE_WDTO			WDTO_15MS	// portUSE_WDTO to use the Watchdog Timer for xTaskIncrementTick


		//	xxx Watchdog Timer is 128kHz nominal, but 120 kHz at 5V DC and 25 degrees is actually more accurate, from data sheet.
		#define configTICK_RATE_HZ		( (TickType_t)( (uint32_t)128000 >> (portUSE_WDTO + 11) ) )  // 2^11 = 2048 WDT scaler for 128kHz Timer
	#elif defined ( portUSE_TIMER3 )
		// portUSE_TIMER3 to use 16 bit Timer3
		#define configTICK_RATE_HZ		( ( TickType_t ) 500 ) // Use 500Hz for TIMER3
		// Use 1000Hz to get mSec timing.
	#elif defined ( portUSE_TIMER0 )
		// portUSE_TIMER0 to use 8 bit Timer0
		#define configTICK_RATE_HZ		( ( TickType_t ) 200  ) // Use 200Hz for TIMER0 and 400Hz for TIMER1
		// Use 1000Hz to get mSec timing.
	#else
		#error "Undefined configTICK_RATE_HZ macro, please, define HERE your own"
	#endif
#endif // configTICK_RATE_HZ

/*-----------------------------------------------------------*/

void initVariant(void);

void vApplicationIdleHook( void );

void vApplicationMallocFailedHook( void );
void vApplicationStackOverflowHook( TaskHandle_t xTask, portCHAR *pcTaskName );

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize );
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize );

/*-----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif // freeRTOSVariant_h
