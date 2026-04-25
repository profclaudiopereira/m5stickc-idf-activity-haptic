#include <stdio.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_timer.h"
#include "esp_log.h"

#include <M5Unified.h>

#include "vibrator.h"

#define TAG "APP"

// ==========================
// CONFIG
// ==========================

#define THRESHOLD 0.05
#define TIME_ALERT_MS 2000   // para teste rápido

// ==========================
// ESTADOS
// ==========================

typedef enum {
    STATE_CALM,
    STATE_ALERT
} state_t;

// ==========================
// UTILS
// ==========================

float calc_magnitude(float x, float y, float z) {
    return sqrtf(x*x + y*y + z*z);
}

// ==========================
// APP
// ==========================

extern "C" void app_main(void)
{
    auto cfg = M5.config();
    M5.begin(cfg);
    M5.Display.setRotation(1);

    vibrator_init();

    ESP_LOGI(TAG, "Fase 2 iniciada");

    state_t state = STATE_CALM;

    float last_accel = 0;
    int64_t movement_start = 0;
    int64_t still_start = 0;

    while (1) {

        float ax, ay, az;
        M5.Imu.getAccel(&ax, &ay, &az);

        float accel = calc_magnitude(ax, ay, az);
        float delta = fabs(accel - last_accel);

        int64_t now = esp_timer_get_time() / 1000;

        ESP_LOGI(TAG, "ACCEL: %.2f | DELTA: %.2f", accel, delta);

        // ==========================
        // DETECÇÃO
        // ==========================

        if (delta > THRESHOLD) {
            if (movement_start == 0) movement_start = now;
            still_start = 0;
        } else {
            if (still_start == 0) still_start = now;
            movement_start = 0;
        }

        // ==========================
        // ESTADOS
        // ==========================

        switch (state) {

            case STATE_CALM:

                M5.Display.fillScreen(TFT_BLACK);
                M5.Display.setCursor(10, 30);
                M5.Display.setTextColor(TFT_WHITE);
		M5.Display.setTextSize(2);
                M5.Display.printf("Tudo Calmo");

                if (movement_start && (now - movement_start > TIME_ALERT_MS)) {
                    state = STATE_ALERT;
                    ESP_LOGI(TAG, "→ ALERTA");
                }

                break;

            case STATE_ALERT:

                M5.Display.fillScreen(TFT_RED);
                M5.Display.setCursor(10, 30);
                M5.Display.setTextColor(TFT_WHITE);
		M5.Display.setTextSize(2);
                M5.Display.printf("ALERTA!");

                vibrator_alert();   // 🔥 vibra

                if (still_start && (now - still_start > TIME_ALERT_MS)) {
                    state = STATE_CALM;
                    ESP_LOGI(TAG, "→ CALMO");
                }

                break;
        }

        last_accel = accel;

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}