#pragma once

#include "Arduino_FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "timers.h"

// This one is mine. lol
#include "FreeRTOS_Utils.h"

#if ARDUINO >= 100
#include <Arduino.h>
#endif
