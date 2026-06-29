#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TM1637_CLK_PIN GPIO_NUM_4
#define TM1637_DIO_PIN GPIO_NUM_6

#define MOEDA_DETECTADA_STR "MOEDA_DET"
#define TIME_RUNNING_STR "TEMPO_R"
#define WELCOME_STR "PIXCAv4"

#define TIMER_START    (1 << 0)
#define TIMER_INCREASE (1 << 1)
#define TIMER_IDLE     (1 << 2)

typedef enum {
    TIMER_IDLE_DEF,
    TIMER_START_DEF,
    TIMER_INCREASE_DEF
} timer_state_t;

extern TaskHandle_t somarDisplay;
extern TaskHandle_t timer;
extern int moeda; 
extern int pulsos;
extern int current_moeda;
extern int tempo_restante; 
extern int timer_seconds; 
extern int running_timer; 