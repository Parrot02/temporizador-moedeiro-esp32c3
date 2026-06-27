#include "tm1637.h"
#include "esp_log.h"
#include "globals.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_log.h>

extern tm1637_handle_t display;

void display_init();
void somar(void *params); 