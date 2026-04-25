#include "vibrator.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define VIB_PIN 26

void vibrator_init(void) {
    gpio_set_direction((gpio_num_t)VIB_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level((gpio_num_t)VIB_PIN, 0);
}

void vibrator_alert(void) {
    gpio_set_level((gpio_num_t)VIB_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(200));
    gpio_set_level((gpio_num_t)VIB_PIN, 0);
}