# 🚀 M5StickC Plus2 - ESP-IDF Activity Monitor

## 📌 Fase 2 --- Detecção de Movimento + Sistema de Alerta com Vibração

------------------------------------------------------------------------

# 🇧🇷 Português

## 🎯 Objetivo

Nesta fase evoluímos o projeto para um sistema **real de monitoramento
de atividade**, capaz de:

-   Detectar movimento usando o acelerômetro (IMU)
-   Identificar períodos contínuos de movimento
-   Disparar alerta visual + vibração
-   Retornar ao estado calmo automaticamente

------------------------------------------------------------------------

## 🧠 Visão geral do funcionamento

Fluxo do sistema:

    Tudo Calmo
       ↓ (movimento contínuo)
    ALERTA + Vibração
       ↓ (sem movimento)
    Tudo Calmo

------------------------------------------------------------------------

## 🧱 Arquitetura do projeto

    main/
    ├── main.cpp
    ├── vibrator.c
    ├── vibrator.h

------------------------------------------------------------------------

## ⚙️ Conceitos utilizados

### 1. Leitura do acelerômetro

``` cpp
float ax, ay, az;
M5.Imu.getAccel(&ax, &ay, &az);
```

------------------------------------------------------------------------

### 2. Cálculo da magnitude

``` cpp
float accel = sqrtf(ax*ax + ay*ay + az*az);
```

👉 Representa a "força total" do movimento

------------------------------------------------------------------------

### 3. Cálculo do delta (variação)

``` cpp
float delta = fabs(accel - last_accel);
```

👉 Detecta mudança entre leituras

------------------------------------------------------------------------

### 4. Threshold (sensibilidade)

``` cpp
#define THRESHOLD 0.05
```

-   menor = mais sensível
-   maior = menos sensível

------------------------------------------------------------------------

### 5. Controle de tempo

``` cpp
int64_t now = esp_timer_get_time() / 1000;
```

------------------------------------------------------------------------

## 🔁 Máquina de estados

``` cpp
typedef enum {
    STATE_CALM,
    STATE_ALERT
} state_t;
```

------------------------------------------------------------------------

## 💻 Código completo (main.cpp comentado)

``` cpp
#include <stdio.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_timer.h"
#include "esp_log.h"

#include <M5Unified.h>

#include "vibrator.h"

#define TAG "APP"

// Sensibilidade do movimento
#define THRESHOLD 0.05

// Tempo necessário para disparar alerta (ms)
#define TIME_ALERT_MS 2000

typedef enum {
    STATE_CALM,
    STATE_ALERT
} state_t;

// Calcula magnitude do vetor
float calc_magnitude(float x, float y, float z) {
    return sqrtf(x*x + y*y + z*z);
}

extern "C" void app_main(void)
{
    auto cfg = M5.config();
    M5.begin(cfg);
    M5.Display.setRotation(1);

    // Inicializa vibrador
    vibrator_init();

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

        // DETECÇÃO DE MOVIMENTO
        if (delta > THRESHOLD) {
            if (movement_start == 0) movement_start = now;
            still_start = 0;
        } else {
            if (still_start == 0) still_start = now;
            movement_start = 0;
        }

        // MÁQUINA DE ESTADOS
        switch (state) {

            case STATE_CALM:

                M5.Display.fillScreen(TFT_BLACK);
                M5.Display.setCursor(10, 30);
                M5.Display.setTextColor(TFT_WHITE);
                M5.Display.setTextSize(2);
                M5.Display.printf("Tudo Calmo");

                if (movement_start && (now - movement_start > TIME_ALERT_MS)) {
                    state = STATE_ALERT;
                }

                break;

            case STATE_ALERT:

                M5.Display.fillScreen(TFT_RED);
                M5.Display.setCursor(10, 30);
                M5.Display.setTextColor(TFT_WHITE);
                M5.Display.setTextSize(2);
                M5.Display.printf("ALERTA!");

                // Vibração
                vibrator_alert();

                if (still_start && (now - still_start > TIME_ALERT_MS)) {
                    state = STATE_CALM;
                }

                break;
        }

        last_accel = accel;

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
```

------------------------------------------------------------------------

## 🔌 Vibração

### vibrator.h

``` c
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void vibrator_init(void);
void vibrator_alert(void);

#ifdef __cplusplus
}
#endif
```

------------------------------------------------------------------------

### vibrator.c

``` c
#include "vibrator.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define VIB_PIN 26

void vibrator_init(void) {
    gpio_set_direction((gpio_num_t)VIB_PIN, GPIO_MODE_OUTPUT);
}

void vibrator_alert(void) {
    gpio_set_level((gpio_num_t)VIB_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(200));
    gpio_set_level((gpio_num_t)VIB_PIN, 0);
}
```

------------------------------------------------------------------------

## ⚠️ Problemas comuns

-   Não entra em ALERTA → threshold alto
-   Não vibra → GPIO não configurado
-   Erro linker → falta extern "C"

------------------------------------------------------------------------

## 🧠 Aprendizados

-   Máquina de estados
-   Processamento de sinal
-   Integração C + C++
-   Controle de hardware (GPIO)

------------------------------------------------------------------------

## 🚀 Próxima fase

-   Vibração não bloqueante
-   UI melhorada
-   Filtros de ruído

------------------------------------------------------------------------

# 🇺🇸 English

## 🎯 Objective

Implement:

-   Motion detection
-   State machine
-   Display feedback
-   Vibration alert

------------------------------------------------------------------------

## 🧠 Core logic

-   Read IMU
-   Compute magnitude
-   Detect change (delta)
-   Track time
-   Trigger alert

------------------------------------------------------------------------

## 🧠 Learnings

-   Embedded state machines
-   Sensor processing
-   C/C++ integration
-   Hardware control

------------------------------------------------------------------------

## 🚀 Next phase

-   Non-blocking vibration
-   Better UI
-   Signal filtering
