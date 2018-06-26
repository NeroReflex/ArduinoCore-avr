/*
  main.cpp - Main loop for Arduino sketches
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Arduino.h>

// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (* /*func*/ )()) { return 0; }

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

void setupUSB() __attribute__((weak));
void setupUSB() { }

aDefineStaticTask(Serial, (15 + configMINIMAL_STACK_SIZE)); // smallest tested: +15
aDefineStaticTask(Main, (15 + configMINIMAL_STACK_SIZE)); // smallest tested: +15

void SerialEvents(void *pvParameters);
void MainLoop(void *pvParameters);

int main(void)
{
	init();

	initVariant();

#if defined(USBCON)
	USBDevice.attach();
#endif
	
	setup();
  
  // Create a task used specifically for serial stuff with MINIMAL priority
  aCreateTask(Serial, SerialEvents, NULL, (configMAX_PRIORITIES - 3));

  // Create the default task with middle priority to retain sketch compatibility
  aCreateTask(Main, MainLoop, NULL, (configMAX_PRIORITIES - 2));

  // Start the real time scheduler.
  vTaskStartScheduler();

  // Probably we've failed trying to initialise heap for the scheduler. Let someone know.
  vApplicationMallocFailedHook();
  
  // Hope this will never happen :)
	return 0;
}



void MainLoop(void *pvParameters __attribute__((unused))) {
  TickType_t xLastWakeTime = xTaskGetTickCount();

  for (;;) {
    // A few seconds delay to call the idle task (it frees memory somehow..... somewhere.....)
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS( 50 ));

    loop();
  }
}


void SerialEvents(void *pvParameters __attribute__((unused))) {
  TickType_t xLastWakeTime = xTaskGetTickCount();

  for (;;) {
    // A few seconds delay to call the idle task (it frees memory somehow..... somewhere.....)
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS( 25 ));

    if (serialEventRun) serialEventRun();
  }
}

