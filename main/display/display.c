#include "display.h"

static const char *TAG = "DISPLAY";
tm1637_handle_t display;
void display_init(){
    tm1637_config_t config = {
        .clk_pin = TM1637_CLK_PIN,
        .dio_pin = TM1637_DIO_PIN,
        .bit_delay_us = 100 // Default timing
    };

    // Initialize the display
    esp_err_t ret = tm1637_init(&config, &display);
    tm1637_set_brightness(display, 0, true);
    vTaskDelay(pdMS_TO_TICKS(500));
    tm1637_show_number_hex(display, 0, true, 4, 0);

    ESP_LOGI(TAG, "Display inicializado com sucesso!");
}

void somar(void *params){
    tm1637_show_number(display, current_moeda, true, 4, 0);
    vTaskDelete(NULL); 
}