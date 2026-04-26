# 🚀 M5StickC Plus2 - ESP-IDF Activity Monitor

Projeto de estudo utilizando **ESP-IDF + M5Unified** no M5StickC Plus2.\
Study project using **ESP-IDF + M5Unified** on M5StickC Plus2.

------------------------------------------------------------------------

# 🇧🇷 Português

## 🎯 Objetivo

Criar um projeto mínimo funcional para:

-   Validar ambiente ESP-IDF
-   Integrar biblioteca M5Unified
-   Ler dados do acelerômetro (IMU)
-   Exibir dados na tela

------------------------------------------------------------------------

## 🧱 Pré-requisitos

-   ESP-IDF instalado (v5.x)
-   Python configurado
-   M5StickC Plus2 conectado via USB

------------------------------------------------------------------------

## ⚙️ Passo a passo

### 1. Criar o projeto

``` bash
idf.py create-project m5stickc-idf-activity-haptic
cd m5stickc-idf-activity-haptic
```

------------------------------------------------------------------------

### 2. Adicionar biblioteca

``` bash
idf.py add-dependency "m5stack/M5Unified"
```

------------------------------------------------------------------------

### 3. Configurar arquivos

#### main/idf_component.yml

``` yaml
dependencies:
  m5stack/M5Unified: "^0.2.13"
```

#### main/CMakeLists.txt

``` cmake
idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "."
)
```

------------------------------------------------------------------------

### 4. Código principal

``` cpp
#include <M5Unified.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// IMPORTANTE:
// extern "C" evita erro de link (C vs C++)
extern "C" void app_main(void)
{
    // Inicializa sistema M5
    auto cfg = M5.config();
    M5.begin(cfg);

    // Rotaciona tela
    M5.Display.setRotation(1);

    while (1) {

        float ax, ay, az;

        // Leitura do acelerômetro
        M5.Imu.getAccel(&ax, &ay, &az);

        // Limpa tela
        M5.Display.fillScreen(TFT_BLACK);

        // Define posição do cursor
        M5.Display.setCursor(10, 10);

        // Exibe valores
        M5.Display.printf("AX: %.2f\n", ax);
        M5.Display.printf("AY: %.2f\n", ay);
        M5.Display.printf("AZ: %.2f\n", az);

        // Aguarda 500ms
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
```

------------------------------------------------------------------------

### 5. Build e upload

``` bash
idf.py build
idf.py flash monitor
```

------------------------------------------------------------------------

## ✅ Resultado esperado

-   Valores AX / AY / AZ mudam ao movimentar o dispositivo
-   Tela atualiza continuamente

------------------------------------------------------------------------

## ⚠️ Problemas comuns

### ❌ app_main undefined

✔ Solução:

``` cpp
extern "C" void app_main(void)
```

------------------------------------------------------------------------

### ❌ M5Unified requires C++

✔ Solução: - Renomear `main.c` → `main.cpp`

------------------------------------------------------------------------

### ❌ Rodar build dentro de /main

✔ Solução: - Executar na raiz do projeto

------------------------------------------------------------------------

## 🧠 Aprendizados

-   Estrutura do ESP-IDF
-   Integração de bibliotecas
-   Diferença entre C e C++
-   Uso básico da IMU

------------------------------------------------------------------------

## 🚀 Próxima fase

-   Detectar movimento
-   Detectar inatividade
-   Criar alertas
-   Ativar vibração

------------------------------------------------------------------------

# 🇺🇸 English

## 🎯 Objective

Create a minimal working project to:

-   Validate ESP-IDF setup
-   Integrate M5Unified
-   Read IMU data
-   Display data on screen

------------------------------------------------------------------------

## 🧱 Prerequisites

-   ESP-IDF installed (v5.x)
-   Python configured
-   M5StickC Plus2 connected via USB

------------------------------------------------------------------------

## ⚙️ Step by step

### 1. Create project

``` bash
idf.py create-project m5stickc-idf-activity-haptic
cd m5stickc-idf-activity-haptic
```

------------------------------------------------------------------------

### 2. Add dependency

``` bash
idf.py add-dependency "m5stack/M5Unified"
```

------------------------------------------------------------------------

### 3. Configure files

#### main/idf_component.yml

``` yaml
dependencies:
  m5stack/M5Unified: "^0.2.13"
```

#### main/CMakeLists.txt

``` cmake
idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "."
)
```

------------------------------------------------------------------------

### 4. Main code

``` cpp
#include <M5Unified.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// IMPORTANT:
// extern "C" avoids linker errors
extern "C" void app_main(void)
{
    auto cfg = M5.config();
    M5.begin(cfg);

    M5.Display.setRotation(1);

    while (1) {

        float ax, ay, az;

        M5.Imu.getAccel(&ax, &ay, &az);

        M5.Display.fillScreen(TFT_BLACK);
        M5.Display.setCursor(10, 10);

        M5.Display.printf("AX: %.2f\n", ax);
        M5.Display.printf("AY: %.2f\n", ay);
        M5.Display.printf("AZ: %.2f\n", az);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
```

------------------------------------------------------------------------

### 5. Build and flash

``` bash
idf.py build
idf.py flash monitor
```

------------------------------------------------------------------------

## ✅ Expected result

-   AX / AY / AZ values change when moving
-   Display updates continuously

------------------------------------------------------------------------

## ⚠️ Common issues

### ❌ app_main undefined

✔ Fix:

``` cpp
extern "C" void app_main(void)
```

------------------------------------------------------------------------

### ❌ M5Unified requires C++

✔ Fix: - Rename `main.c` → `main.cpp`

------------------------------------------------------------------------

### ❌ Running build inside /main

✔ Fix: - Run from project root

------------------------------------------------------------------------

## 🧠 Learnings

-   ESP-IDF structure
-   Library integration
-   C vs C++ differences
-   Basic IMU usage

------------------------------------------------------------------------

## 🚀 Next phase

-   Detect movement
-   Detect inactivity
-   Create alerts
-   Trigger vibration
