/*
 * FAT Informática LTDA Laboratório de Desenvolvimento
 * Parrot02 - ESP32-C3 - Co-processador de moedas - Pixca
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "temporizador.h"
#include "globals.h"
#include "moedeiro.h"
#include "display.h"
#include "esp_flash.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include <esp_err.h>

QueueHandle_t uart_queue;

const int uart_buffer_size = (1024 * 2);

uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
    .rx_flow_ctrl_thresh = 122,
};

void app_main(void)
{
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, GPIO_NUM_21, GPIO_NUM_20, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    xTaskCreate(coin_gpio_init, "coin_gpio_init", 4096, NULL, 5, NULL);
    xTaskCreate(start_timer, "start_timer", 4096, NULL, 5, &timer);
    xTaskCreate(somar, "somar", 4096, NULL, 5, &somarDisplay);
    display_init();
    uart_write_bytes(UART_NUM_1, WELCOME_STR, strlen(WELCOME_STR));
}
