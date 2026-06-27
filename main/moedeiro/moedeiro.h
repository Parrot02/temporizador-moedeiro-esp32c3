#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "driver/uart.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "globals.h"

#define MOEDEIRO_PIN GPIO_NUM_3

void coin_gpio_init(void *params); 
void level(void *params);