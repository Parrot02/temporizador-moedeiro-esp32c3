#include "globals.h"

TaskHandle_t somarDisplay = NULL;
TaskHandle_t timer = NULL;

int moeda = 0; 
int pulsos = 2;
int running_timer = 0; 
int current_moeda = 0;
int tempo_restante = 200; 
int timer_seconds = 200; 
