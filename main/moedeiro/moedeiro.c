#include "moedeiro.h"

#define SAMPLE_PERIOD_US 100            // 100 us = 10 kHz // 100.000
#define LOW_MIN_US       50000          // 50 ms // 50000
#define LOW_MIN_SAMPLES  (LOW_MIN_US / SAMPLE_PERIOD_US)

static const char *TAG = "MOEDEIRO";

volatile uint16_t lowSamples = 0;
volatile bool pulseActive = false;
volatile uint16_t coinCount = 0;
volatile uint16_t coinDump = 0; 

static bool IRAM_ATTR gptimer_coin_isr(gptimer_handle_t timer,
                                       const gptimer_alarm_event_data_t *edata,
                                       void *user_ctx)
{
    bool level = gpio_get_level(MOEDEIRO_PIN); // HIGH ou LOW

    if (level == 0) { // LOW
        if (lowSamples < 65000) {
            lowSamples++;
        }
    } else { // HIGH
        if (pulseActive) {
            pulseActive = false;
            lowSamples = 0;
        }
    }

    // Quando atingir largura mínima → pulso válido
    if (!pulseActive && lowSamples >= LOW_MIN_SAMPLES) {
        pulseActive = true;
        coinCount++;
        coinDump++; 
    }

    return true; // manter alarme periódico ativo
}

static gptimer_handle_t setup_timer_10khz(void)
{
    gptimer_handle_t timer = NULL;

    // Configuração básica do timer
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT, // 80 MHz no ESP32
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000, // 1 MHz → 1 tick = 1 µs
    };

    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &timer));

    // Registrar callback (ISR)
    gptimer_event_callbacks_t cbs = {
        .on_alarm = gptimer_coin_isr,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(timer, &cbs, NULL));

    // Configurar alarme periódico em 100 µs
    gptimer_alarm_config_t alarm_config = {
        .alarm_count = SAMPLE_PERIOD_US, // 100 us
        .reload_count = 0,
        .flags.auto_reload_on_alarm = true,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(timer, &alarm_config));

    // Habilitar e iniciar
    ESP_ERROR_CHECK(gptimer_enable(timer));
    ESP_ERROR_CHECK(gptimer_start(timer));

    ESP_LOGI(TAG, "GPTimer iniciado a %d us", SAMPLE_PERIOD_US);

    return timer;
}

void moedeiro_listener(void *params)
{
    ESP_LOGI(TAG, "Listener iniciado!");
    while (1) {
        if(coinDump != 0){
            coinDump = 0; 
            current_moeda += 1; 
        }
        if (coinCount >= pulsos) {
            ESP_LOGI(TAG, "Moeda detectada! Total: %d", coinCount);
            uart_write_bytes(UART_NUM_1, MOEDA_DETECTADA_STR, strlen(MOEDA_DETECTADA_STR));
            coinCount = 0; 
            current_moeda = 0; 
            moeda = 1; 
            // TO DO xTaskNotify "RUN TIMER"
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }

}

void coin_gpio_init(void *params)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << MOEDEIRO_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,   // típico para moedeiro open-collector
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
    gptimer_handle_t timer = setup_timer_10khz();
    xTaskCreate(moedeiro_listener, "moedeiro_listener", 4096, NULL, 7, NULL);

    ESP_LOGI(TAG, "Moedeiro inicializado no pino %d", MOEDEIRO_PIN);
    vTaskDelete(NULL); 
}