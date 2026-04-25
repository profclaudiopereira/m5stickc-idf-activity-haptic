#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <M5Unified.h>

// ==========================
// APP MAIN (C + C++)
// ==========================

extern "C" void app_main(void)
{
    // Inicializa M5 (display + IMU + etc)
    auto cfg = M5.config();
    M5.begin(cfg);

    // Ajusta orientação da tela
    M5.Display.setRotation(1);

    // Tela inicial
    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setTextColor(TFT_GREEN);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Inicializando...");

    vTaskDelay(pdMS_TO_TICKS(1000));

    // ==========================
    // LOOP PRINCIPAL
    // ==========================
    while (1) {

        float ax, ay, az;

        // Leitura da IMU
        M5.Imu.getAccel(&ax, &ay, &az);

        // Limpa tela
        M5.Display.fillScreen(TFT_BLACK);

        // Texto
        M5.Display.setTextColor(TFT_WHITE);
        M5.Display.setCursor(10, 10);

        M5.Display.println("Tudo Calmo");
        M5.Display.println("");

        // Mostra valores
        M5.Display.printf("AX: %.2f\n", ax);
        M5.Display.printf("AY: %.2f\n", ay);
        M5.Display.printf("AZ: %.2f\n", az);

        // Delay
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}