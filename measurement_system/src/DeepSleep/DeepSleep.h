#include "esp_sleep.h"
#include "ESP32Time.h"
#define uS_TO_S 1000000
#define TIME_TO_SLEEP 600.0 // Time in seconds

double time_to_sleep = uS_TO_S * TIME_TO_SLEEP;