#include "moedeiro.h"

#define GPIO_INPUT_PIN_SEL  (1ULL << MOEDEIRO_PIN) // Configura o pino 0
#define ESP_INTR_FLAG_DEFAULT 0

static const char *TAG = "MOEDEIRO";

volatile uint16_t lowSamples = 0;
volatile bool pulseActive = false;
volatile uint16_t coinCount = 0;
volatile uint16_t coinDump = 0; 

TaskHandle_t moedeiroTaskHandle; 

static void IRAM_ATTR gpio_isr_handler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    uint64_t last_falling;
    // Faça algo aqui (ex: enviar notificação para uma task via fila)
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR(
        moedeiroTaskHandle,
        &xHigherPriorityTaskWoken
    );

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void moedeiro_listener(void *params)
{
    ESP_LOGI(TAG, "Listener iniciado!");
    while (1)
        {
            // Espera indefinidamente pela interrupção
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

            ESP_LOGI(TAG, "Pulso detectado!");
            uart_write_bytes(UART_NUM_1, MOEDA_DETECTADA_STR, strlen(MOEDA_DETECTADA_STR));

            coinCount++;
            vTaskDelay(pdMS_TO_TICKS(100));
        }
}

void coin_gpio_init(void *params)
{
    xTaskCreate(moedeiro_listener, "moedeiro_listener", 4096, NULL, 7, &moedeiroTaskHandle);
    // 1. Configurar os parâmetros do GPIO
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_NEGEDGE,        // Interrupção por borda de descida (quando o botão é pressionado)
        .mode = GPIO_MODE_INPUT,               // Modo de entrada
        .pin_bit_mask = GPIO_INPUT_PIN_SEL,    // Pinos selecionados
        .pull_up_en = 1,                       // Habilita resistor pull-up interno
        .pull_down_en = 0                      // Desabilita pull-down
    };
    
    // Aplica a configuração
    gpio_config(&io_conf);

    // 2. Instalar o serviço ISR GPIO
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    // 3. Adicionar o manipulador (handler) da interrupção para o pino específico
    gpio_isr_handler_add(MOEDEIRO_PIN, gpio_isr_handler, (void*) MOEDEIRO_PIN);

    ESP_LOGI(TAG, "Moedeiro inicializado no pino %d", MOEDEIRO_PIN);

    while (1) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void level(void *params){
    while(1){
        ESP_LOGI(TAG, "Level %d", gpio_get_level(MOEDEIRO_PIN));
        vTaskDelay(500);
    }
}