idf_component_register(SRCS "hello_world_main.c" "moedeiro/moedeiro.c" "globals/globals.c" "temporizador/temporizador.c" "display/display.c"
                       PRIV_REQUIRES spi_flash
                       PRIV_REQUIRES nvs_flash
                       PRIV_REQUIRES esp_driver_gptimer
                       PRIV_REQUIRES esp_timer
                       PRIV_REQUIRES esp_driver_uart
                       INCLUDE_DIRS "" "moedeiro" "globals" "temporizador" "display")
