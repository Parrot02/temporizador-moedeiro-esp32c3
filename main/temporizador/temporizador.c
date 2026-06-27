#include "temporizador.h"

void start_timer(void *params){    
    uint32_t evt;
    timer_state_t state = TIMER_IDLE_DEF;

    for(;;){
        xTaskNotifyWait(
            0,
            ULONG_MAX,              // ⬅️ NÃO limpa bits aqui
            &evt,
            portMAX_DELAY
        );
        /* Processa eventos */
        if (evt & TIMER_IDLE) {
            state = TIMER_IDLE_DEF;
            tempo_restante = 0; 
            continue;
        }

        if(evt & TIMER_INCREASE){
            state = TIMER_INCREASE_DEF;
            tempo_restante += timer_seconds;
        }

        if (evt & TIMER_START) {
            state = TIMER_START_DEF;
            tempo_restante = timer_seconds;
            // Implementação do temporizador
            tm1637_clear(display);
            tm1637_set_brightness(display, 7, true);

            uint32_t last_time = esp_timer_get_time() / 1000;

            while(tempo_restante > 0){
                if(esp_timer_get_time() / 1000 >= 1000){
                    tempo_restante--;
                    last_time += 1000;

                    int min = tempo_restante / 60; 
                    int sec = tempo_restante % 60;

                    tm1637_show_number(display, sec, true, 4, 0);
                    tm1637_show_number(display, min, false, 1, 1);
                    vTaskDelay(pdMS_TO_TICKS(1000));
                }
            }
            
            tempo_restante = timer_seconds; 
            
            tm1637_clear(display);
            tm1637_set_brightness(display, 0, true);
            tm1637_show_number_hex(display, 0, true, 4, 0);
            
        }
    }

}